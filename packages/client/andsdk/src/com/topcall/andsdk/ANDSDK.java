package com.topcall.andsdk;

import com.topcall.andsdk.login.LoginMgr;

public class ANDSDK {
	private LoginMgr mLoginMgr = null;
	
	public ANDSDK() {
		mLoginMgr = new LoginMgr(this);
	}
	
	public LoginMgr	getLoginMgr() {
		return mLoginMgr;
	}
	
	
}
