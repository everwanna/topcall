package com.topcall.andsdk.login;

import com.topcall.andsdk.protobase.IProtoLinkHandler;
import com.topcall.andsdk.protobase.ProtoLink;
import com.topcall.andsdk.protobase.ProtoLog;

public class LoginLink implements IProtoLinkHandler {	
	private LoginMgr mLoginMgr = null;
	private ProtoLink mLink = null;
	
	public LoginLink(LoginMgr mgr) {
		mLoginMgr = mgr;
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
	public void onDisconnected() {
		ProtoLog.log("MCProxyLink.onDisconnected.");
	}

	@Override
	public void onTimer(int id) {
		//ProtoLog.log("AudioUdpLink.onTimer.");
	}

	@Override
	public void onData(int uri, byte[] buf) {
		mLoginMgr.getProtoHandler().onProto(uri, buf);
	}

}
