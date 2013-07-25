package com.topcall.andsdk.login;

import com.topcall.andsdk.login.proto.PLoginRes;
import com.topcall.andsdk.protobase.ProtoLog;
import com.topcall.andsdk.protobase.ProtoUris;

public class LoginProtoHandler {
	private LoginMgr mCallMgr = null;
	
	public LoginProtoHandler(LoginMgr mgr) {
		this.mCallMgr = mgr;
	}
	
	public void onProto(int uri, byte[] buf) {
		switch(uri) {
		case ProtoUris.URI_LOGIN_RES:
			break;
		}
	}	
	
	private void onLoginRes(byte[] buf) {
		PLoginRes res = new PLoginRes();
		res.unmarshall(buf);
		
		ProtoLog.log("LoginProtoHandler.onLoginRes, res/uid=" + res.res + "," + res.uid);
	}
}
