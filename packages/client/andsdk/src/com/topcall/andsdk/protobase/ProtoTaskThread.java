package com.topcall.andsdk.protobase;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;

public class ProtoTaskThread{
	private static ProtoTaskThread sInstance = null;
	static {
		sInstance = new ProtoTaskThread();
	}
	
	public static ProtoTaskThread getInstance() {
		return sInstance;
	}
	
	private HandlerThread mThread = null;
	private Looper mLooper = null;
	private Handler mHandler = null;	
	
	public ProtoTaskThread() {
		mThread = new HandlerThread("protocol");
		mThread.start();
		
		mLooper = mThread.getLooper();
		mHandler = new Handler(mLooper);
	}
	
	public void	post(ProtoTask task) {
		mHandler.post(task);
	}
	
	public void postDelay(ProtoTask task, int delay) {
		mHandler.postDelayed(task, (long)delay);
	}
}
