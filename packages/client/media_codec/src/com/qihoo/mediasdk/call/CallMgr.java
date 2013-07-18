package com.qihoo.mediasdk.call;

import com.qihoo.mediasdk.MediaSDK;
import com.qihoo.mediasdk.call.proto.PMCAllocReq;
import com.qihoo.mediasdk.call.proto.PMGAllocProxyReq;
import com.qihoo.mediasdk.protobase.ProtoLog;

public class CallMgr {
	private MediaSDK mSDK = null;
	private MCProxyLink mMCLink = null;
	private MGroupLink mMGLink = null;
	private CallProtoHandler mProtoHandler = null;
	private int mIsp1 = 0;
	private int mArea1 = 0;
	private int mIsp2 = 0;
	private int mArea2 = 0;
	
	public CallMgr(MediaSDK sdk) {
		mSDK = sdk;
		mMCLink = new MCProxyLink(this);		
		mMGLink = new MGroupLink(this);		
		mProtoHandler = new CallProtoHandler(this);
	}
	
	/**
	 * login into mcproxy.
	 */
	public void login(String mcp_ip, int mcp_port) {
		mMCLink.connect(mcp_ip, mcp_port);
	}
	
	/**
	 * 3 steps for call:
	 * 	a), ask mcproxy for mgroup.
	 *  b), ask mgroup for mproxy.
	 *  c), publish and subscribe to mproxy.
	 * @param uid
	 * @param isp
	 * @param area
	 */
	public void call(int isp1, int area1, int isp2, int area2) {
		ProtoLog.log("CallMgr.call.");
		mIsp1 = isp1;
		mArea1 = area1;
		mIsp2 = isp2;
		mArea2 = area2;
		
		queryGroup();
	}
	
	/**
	 * login to mgroup:
	 * @param ip
	 * @param port
	 */
	public void loginGroup(String ip, int port) {
		mMGLink.connect(ip, port);
	}
	
	public void	queryGroup() {
		ProtoLog.log("CallMgr.queryGroup.");
		PMCAllocReq req = new PMCAllocReq();
		req.isp1 = mIsp1;
		req.area1 = mArea1;
		req.isp2 = mIsp2;
		req.area2 = mArea2;
	
		mMCLink.send(req.marshall());
	}
	
	public void queryProxy() {
		ProtoLog.log("CallMgr.queryProxy.");
		PMGAllocProxyReq req = new PMGAllocProxyReq();
		req.stream="stream1";
		mMGLink.send(req.marshall());
	}
	
	public void loginProxy(String ip, int port) {
		ProtoLog.log("CallMgr.loginProxy, ip/port=" + ip + "," + port);
		//call into audio module:
		mSDK.getAudioMgr().connect(ip, port);
	}
	
	public CallProtoHandler getProtoHandler() {
		return mProtoHandler;
	}
}
