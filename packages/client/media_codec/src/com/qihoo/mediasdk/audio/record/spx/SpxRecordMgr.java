package com.qihoo.mediasdk.audio.record.spx;

import com.qihoo.mediasdk.audio.AudioMgr;
import com.qihoo.mediasdk.audio.record.IRecordListener;
import com.qihoo.mediasdk.audio.record.IRecordMgr;
import com.qihoo.mediasdk.codec.Speex;

public class SpxRecordMgr implements IRecordMgr{
	private AudioMgr mAudioMgr = null;
	private SpxRecordThread mThread = null;	
	private boolean mStarted = false;
	
	public SpxRecordMgr(AudioMgr mgr, IRecordListener listener) {
		mAudioMgr = mgr;
		mThread = new SpxRecordThread(this, listener);
	}

	@Override
	public void startRecord() {
		if( mStarted )
			return;
		
		mThread.start();
		mStarted = true;
	}

	@Override
	public void stopRecord() {
		if( !mStarted )
			return;
		
		mThread.stop();
		mStarted = false;
	}
	
	public AudioMgr getAudioMgr() {
		return mAudioMgr;
	}
}
