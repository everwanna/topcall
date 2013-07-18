package com.qihoo.mediasdk.call;

import com.qihoo.mediasdk.protobase.IProtoLinkHandler;
import com.qihoo.mediasdk.protobase.ProtoLink;
import com.qihoo.mediasdk.protobase.ProtoLog;

public class MCProxyLink implements IProtoLinkHandler {

	private CallMgr mCallMgr = null;
	private ProtoLink mLink = null;
	
	public MCProxyLink(CallMgr mgr) {
		mCallMgr = mgr;
		mLink = new ProtoLink(true, this);
		mLink.addTimer(0, 10000);
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
		ProtoLog.log("MCProxyLink.onConnected.");
	}

	@Override
	public void onData(short svid, short uri, byte[] buf) {
		mCallMgr.getProtoHandler().onProto(svid, uri, buf);
	}

	@Override
	public void onDisconnected() {
		ProtoLog.log("MCProxyLink.onDisconnected.");
	}

	@Override
	public void onTimer(int id) {
		//ProtoLog.log("AudioUdpLink.onTimer.");
	}

}
