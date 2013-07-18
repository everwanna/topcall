package com.qihoo.mediasdk.audio.record;

import java.io.IOException;

import com.qihoo.mediasdk.protobase.ProtoLog;

import android.media.MediaRecorder;
import android.media.MediaRecorder.AudioEncoder;
import android.media.MediaRecorder.AudioSource;
import android.media.MediaRecorder.OutputFormat;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.net.LocalSocketAddress;

public class RecordMgr {
	private IRecordListener mListener = null;
	private MediaRecorder mRecorder = null;	
	private LocalSocket mSocket = null;
	private LocalSocket mReciver = null;
	private LocalServerSocket mServerSocket = null;
	private RecordThread mThread = null;
	private boolean mStarted = false;
	
	public RecordMgr(IRecordListener listener) {
		mListener = listener;
		mRecorder = new MediaRecorder();
		mSocket = new LocalSocket();
		try {
			mServerSocket = new LocalServerSocket("audio_socket");			
		} catch (IOException ex) {
			log("RecordMgr.RecordMgr, exception=" + ex.getMessage());
		}
	}
	
	public void startRecord() {		
		ProtoLog.log("RecordMgr.startRecord.");
		
		//in case user press start again:
		if( mStarted )
			return;
		try {			
			mSocket.connect( new LocalSocketAddress("audio_socket"));
			mReciver = mServerSocket.accept();
			
			mRecorder.setAudioSource(AudioSource.MIC);
			mRecorder.setOutputFormat( OutputFormat.AMR_WB );
			mRecorder.setAudioEncoder(AudioEncoder.AMR_WB);
			mRecorder.setAudioEncodingBitRate(AMRWBConfig.MODE_4);
			
			mRecorder.setOutputFile(mSocket.getFileDescriptor());
			mRecorder.prepare();
			
			//start the RecordThread:			
			mThread = new RecordThread(4, mListener, mReciver);
			mThread.start();
		} catch(Exception ex) {
			log("RecordMgr.startRecord, exception=" + ex.getMessage());
		}
		
		mRecorder.start();
		mStarted = true;
	}
	
	public void stopRecord() {
		mRecorder.stop();
		mStarted = true;
		try {
			mSocket.close();
			mReciver.close();
			mServerSocket.close();			
			mThread.stop();			
		} catch (IOException ex) {
			log("RecordMgr.stopRecord, exception=" + ex.getMessage());
		}
		
	}
	
	public IRecordListener getListener() {
		return mListener;
	}
	
	private void log(String msg) {
		System.out.println("MEDIA - " + msg);
	}
}
