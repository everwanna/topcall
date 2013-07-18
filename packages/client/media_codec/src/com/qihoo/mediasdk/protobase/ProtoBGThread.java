package com.qihoo.mediasdk.protobase;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;

public class ProtoBGThread {
	private static ProtoBGThread sInstance = null;
	static {
		sInstance = new ProtoBGThread();
	}
	
	public static ProtoBGThread getInstance() {
		return sInstance;
	}
	
	private HandlerThread mThread = null;
	private Looper mLooper = null;
	private Handler mHandler = null;	
	
	public ProtoBGThread() {
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
