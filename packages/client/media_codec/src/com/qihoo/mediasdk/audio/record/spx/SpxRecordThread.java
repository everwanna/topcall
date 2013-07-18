package com.qihoo.mediasdk.audio.record.spx;

import com.qihoo.mediasdk.audio.record.IRecordListener;
import com.qihoo.mediasdk.protobase.ProtoLog;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

public class SpxRecordThread extends Thread{
	private SpxRecordMgr mRecordMgr = null;
	private IRecordListener mListener = null;
	private AudioRecord mRecorder = null;
	private int mBufSize = 0;
	private byte[] mBuffer = null;	
	private byte[] mEncBuf = null;	
	
	public SpxRecordThread(SpxRecordMgr mgr, IRecordListener listener) {
		mRecordMgr = mgr;
		mListener = listener;
	}
	
	@Override
	public void run() {	
		mBufSize = AudioRecord.getMinBufferSize(16000, AudioFormat.CHANNEL_CONFIGURATION_MONO,  
				AudioFormat.ENCODING_PCM_16BIT);
		mBuffer = new byte[ mBufSize ];
		mEncBuf = new byte[ mBufSize ];		
		mRecorder = new AudioRecord(MediaRecorder.AudioSource.DEFAULT,
				16000, 
			AudioFormat.CHANNEL_CONFIGURATION_MONO,
			AudioFormat.ENCODING_PCM_16BIT, 
			mBufSize);
	
		mRecorder.startRecording();
		
		int len = 0;
		int enc_len = 0;		
		while(true) {
			len = mRecorder.read(mBuffer, 0, mBufSize);
			if( len != 0 ) {				
				enc_len = mRecordMgr.getAudioMgr().getSilk().encode(mBuffer, len, mEncBuf, mBufSize );				
				//dec_len = mRecordMgr.getCodec().decode(mEncBuf, enc_len, mDecBuf, 320);
				//ProtoLog.log("SpxRecordThread.run, len/enc_len/dec_len=" + len + "," + enc_len + "," + dec_len);
				
				if( mListener != null ) {
					mListener.onFrame(mEncBuf, 0, enc_len);
				}
			}
		}
	}
}	
