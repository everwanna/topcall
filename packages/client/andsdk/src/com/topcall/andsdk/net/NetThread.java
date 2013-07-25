package com.topcall.andsdk.net;

import java.util.LinkedList;
import java.util.List;

public class NetThread extends Thread {
	private NetMgr mNetMgr = null;
	
	public NetThread(NetMgr mgr) {
		this.mNetMgr = mgr;
	}
	
	@Override
	public void run() {
		List<NetMsg> msgs = new LinkedList<NetMsg>();
		
		while(true) {
			mNetMgr.select();
			try {
				Thread.sleep(20);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			mNetMgr.getMsgs(msgs);
			for( int i=0; i<msgs.size(); i++ ) {
				NetMsg msg = msgs.get(i);
				switch(msg.type) {
				case NetMsg.NET_SEND:
					onSend(msg.linkid, msg.buf, msg.offset, msg.len);
					break;
				case NetMsg.NET_CONNECT:
					onConnect(msg.linkid, msg.ip, msg.port);
					break;
				case NetMsg.NET_CLOSE:
					onClose(msg.linkid);
					break;
				}
			}
			msgs.clear();
			
			//check timers:
			mNetMgr.getTimerMgr().check();
		}
	}
	
	private void onConnect(int linkid, String ip, int port) {
		mNetMgr.connectDirect(linkid, ip, port);
	}
	
	private void onSend(int linkid, byte[] buf, int offset, int len) {
		mNetMgr.sendDirect(linkid, buf, offset, len);
	}
	
	private void onClose(int linkid) {
		mNetMgr.closeDirect(linkid);
	}
}
