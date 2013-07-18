package com.qihoo.mediasdk.net;

import java.nio.channels.Selector;

public class NetLinkBase {
	public static final int	PROTO_TCP_BUFFER_SIZE = 128*1024;
	public static final int	PROTO_UDP_BUFFER_SIZE = 64*1024;	
	public static final int PROTO_UDP_PORT = 2001;
	
	protected NetMgr mNetMgr = null;
	protected int mLinkId = -1;
	protected Selector mSelector = null;
	
	protected INetLinkHandler mHandler = null;
	
	public NetLinkBase(NetMgr mgr, int linkid, Selector selector, INetLinkHandler handler) {
		this.mNetMgr = mgr;
		this.mLinkId = linkid;
		this.mSelector = selector;		
		this.mHandler = handler;		
	}
	
	public int getLinkId() {
		return mLinkId;
	}
	
	public void connect(String ip, int port) {
		
	}
	
	public void	send(byte[] buf, int offset, int len) {
	}
	
	public void addTimer(int id, int interval) {
		mNetMgr.addTimer(mLinkId, id, interval);
	}
	
	public void close() {
	}
	
	public void onRead() {
	}
	
	public void onConnected() {
	}
	
	public void onTimer(int id) {
		//NetLog.log("NetLink.onTimer, linkid/id=" + mLinkId + "," + id);
		mHandler.onTimer(id);
	}
}
