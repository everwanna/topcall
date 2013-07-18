package com.qihoo.mediasdk;

import com.qihoo.mediasdk.audio.AudioMgr;
import com.qihoo.mediasdk.call.CallMgr;

public class MediaSDK {
	private IMediaSDKListener mListener = null;
	private	CallMgr mCallMgr = null;
	private AudioMgr mAudioMgr = null;
	
	public MediaSDK(IMediaSDKListener listener, int uid) {
		mListener = listener;
		mCallMgr = new CallMgr(this);
		mAudioMgr = new AudioMgr(uid);
	}
	
	public void login(String mcp_ip, int mcp_port) {
		mCallMgr.login(mcp_ip, mcp_port);
	}
	
	public void	call(int isp1, int area1, int isp2, int area2) {
		mCallMgr.call(isp1, area1, isp2, area2);
	}
	
	public AudioMgr getAudioMgr() {
		return mAudioMgr;
	}
}
