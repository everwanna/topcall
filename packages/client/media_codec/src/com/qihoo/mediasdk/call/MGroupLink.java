package com.qihoo.mediasdk.call;

import com.qihoo.mediasdk.protobase.IProtoLinkHandler;
import com.qihoo.mediasdk.protobase.ProtoLink;
import com.qihoo.mediasdk.protobase.ProtoLog;

public class MGroupLink implements IProtoLinkHandler {
	private CallMgr mCallMgr = null;
	private ProtoLink mLink = null;
	
	public MGroupLink(CallMgr mgr) {
		mCallMgr = mgr;
		mLink = new ProtoLink(true, this);
	}
	
	public void connect(String ip, int port) {
		mLink.connect(ip, port);
	}
	
	public void close() {
		mLink.close();
	}
	
	public void send(byte[] buf) {
		send(buf, 0, buf.length);
	}
	
	public void send(byte[] buf, int offset, int len) {
		mLink.send(buf, offset, len);
	}
	
	@Override
	public void onConnected() {
		ProtoLog.log("MGroupLink.onConnected.");
		mCallMgr.queryProxy();
	}

	@Override
	public void onData(short svid, short uri, byte[] buf) {
		// TODO Auto-generated method stub
		mCallMgr.getProtoHandler().onProto(svid, uri, buf);
	}

	@Override
	public void onDisconnected() {
		// TODO Auto-generated method stub

	}

	@Override
	public void onTimer(int id) {
		// TODO Auto-generated method stub

	}

}
