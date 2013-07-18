package com.qihoo.mediasdk.protobase;

public class ProtoTask implements Runnable {
	private String mName = null;
	
	public ProtoTask() {
		
	}
	
	public ProtoTask(String name) {
		this.mName = name;
	}

	public String getName() {
		return mName;
	}
	
	@Override
	public void run() {
		
	}
	
	/*
	public void post() {
		ProtoTaskThread.getInstance().post(this);
	}
	
	public void postDelay(int delay) {
		ProtoTaskThread.getInstance().postDelay(this, delay);
	}
	*/
}
