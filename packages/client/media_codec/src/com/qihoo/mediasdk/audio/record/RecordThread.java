package com.qihoo.mediasdk.audio.record;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

import com.qihoo.mediasdk.protobase.ProtoLog;

import android.net.LocalSocket;

public class RecordThread extends Thread {
	private IRecordListener mListener = null;	
	private LocalSocket	mSocket = null;
	private byte[] mHeader = null;
	private byte[] mBuffer = null;
	private ByteBuffer mBKBufer = ByteBuffer.allocate(4096);
	private int mEnd = 0;
	private int mStart = 0;	
	private AMRWBConfig mConfig = null;
	
	public RecordThread(int seq, IRecordListener listener, LocalSocket socket) {
		mConfig = new AMRWBConfig(seq);
		mListener = listener;
		mSocket = socket;	
		
		mHeader = new byte[mConfig.getHeaderSize()];
		mBuffer = new byte[mConfig.getFrameSize()*2];
	}
	
	@Override
	public void run() {		
		try {
			int pt = 0;
			int len = 0;
			int frame_len = 0;
			byte byte0 = 0;
			
			InputStream stream = mSocket.getInputStream();
			len  = stream.read(mHeader);
			if( mListener != null ) {
			//	mListener.onFrame(mHeader);
			}
			
			while(true) {				
				len = stream.read(mBuffer);
				if( len == 0 )
					continue;
				mBKBufer.position(mEnd);
				mBKBufer.put(mBuffer, 0, len);
				mEnd = mBKBufer.position();
				
				while(true) {
					mBKBufer.position(mStart);
					byte0 = mBKBufer.get();
					pt = (byte0>>3)&0xF;
					if( pt <0 || pt>15 ) {
						ProtoLog.error("RecordThread.run, invalid pt=" + pt);					
					}					
					frame_len = mConfig.getFrameSize(pt);
					ProtoLog.log("RecordThread.run, byte0/pt/frame_len=" + byte0 + "," + pt + "," + frame_len);
					
					if( frame_len == 0 ) {
						mStart = mBKBufer.position();
						continue;
					} else if( mEnd - mStart >= frame_len ) {						
						mBKBufer.position(mStart);
						mBKBufer.get(mBuffer, 0, frame_len);						
						
						if( mBuffer[0] != mConfig.getByte0() ) {
							//log("RecordThread.run, byte[0] not good, expect/byte[0]=" + mConfig.getByte0() + "," + mBuffer[0]);
						} else {
							//log("RecordThread.run, read a frame.");
							
							if( mListener != null ) {
								mListener.onFrame(mBuffer, 0, frame_len);
							}
						}
						mStart = mBKBufer.position();
					} else {
						break;
					}
				}
				
				if( mStart >= 2048 ) {
					//make a shuffer:
					shuffer();
				}
			}			
		} catch (IOException ex) {
			log("RecordThread.run, exception=" + ex.getMessage());
		}
		
	}
	
	private void shuffer() {
		byte[] temp = new byte[ mEnd-mStart ];
		mBKBufer.get(temp);
		mBKBufer.clear();
		mBKBufer.put(temp);
		mBKBufer.position(0);		
		
		mEnd = mEnd-mStart;
		mStart = 0;
	}
	
	private void log(String msg) {
		System.out.println("MEDIA - " + msg);
	}
}
