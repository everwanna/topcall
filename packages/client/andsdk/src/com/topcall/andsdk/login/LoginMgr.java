package com.topcall.andsdk.login;

import com.topcall.andsdk.ANDSDK;
import com.topcall.andsdk.login.proto.PLoginReq;
import com.topcall.andsdk.protobase.ProtoLog;

public class LoginMgr {
	private ANDSDK mSDK = null;
	private LoginLink mLoginLink = null;	
	private LoginProtoHandler mProtoHandler = null;
	
	public LoginMgr(ANDSDK sdk) {
		mSDK = sdk;
		mLoginLink = new LoginLink(this);	
		mLoginLink.connect("10.18.71.18", 2061);
		mProtoHandler = new LoginProtoHandler(this);
	}
	
	public void login(String passport, String password) {
		PLoginReq req = new PLoginReq();
		req.passport = passport;
		req.password = password;
		mLoginLink.send( req.marshall());
	}
	
	
	public LoginProtoHandler getProtoHandler() {
		return mProtoHandler;
	}
}
