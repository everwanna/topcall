package com.qihoo.mediasdk.net;

import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;

public class NetQueue {
	private ConcurrentLinkedQueue<NetMsg> mMsgList = new ConcurrentLinkedQueue<NetMsg>();
	
	public NetQueue() {
		
	}
	
	public void push(NetMsg msg) {
		mMsgList.add(msg);
	}
	
	public void fetch(List<NetMsg> msgs, int max) {
		int num=0;
		while(num<max && mMsgList.size() > 0 ) {
			msgs.add( mMsgList.poll() );
			num++;
		}
	}
}
