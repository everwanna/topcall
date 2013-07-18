package com.qihoo.mediasdk.call;

import com.qihoo.mediasdk.call.proto.PMCAllocRes;
import com.qihoo.mediasdk.call.proto.PMGAllocProxyRes;
import com.qihoo.mediasdk.protobase.ProtoLog;
import com.qihoo.mediasdk.protobase.ProtoSvid;
import com.qihoo.mediasdk.protobase.ProtoUris;

public class CallProtoHandler {
	private CallMgr mCallMgr = null;
	
	public CallProtoHandler(CallMgr mgr) {
		this.mCallMgr = mgr;
	}
	
	public void onProto(short svid, short uri, byte[] buf) {
		if( svid == ProtoSvid.SVID_MCPROXY || svid == ProtoSvid.SVID_MCENTER ) {
			switch(uri) {
			case ProtoUris.URI_MCENTER_ALLOC_RES:
				onMCAllocRes(buf);
				break;
			
			}
		} else if( svid == ProtoSvid.SVID_MGROUP ) {
			switch(uri) {
			case ProtoUris.URI_MGROUP_ALLOC_PROXY_RES:
				onMGAllocProxyRes(buf);
				break;			
			}
		}
	}	
	
	private void onMCAllocRes(byte[] buf) {
		PMCAllocRes res = new PMCAllocRes();
		res.unmarshall(buf);
		
		ProtoLog.log("CallProtoHandler.onMCAllocRes, ip/port=" + res.ip + "," + res.port);
		mCallMgr.loginGroup(res.ip, res.port);
	}
	
	private void onMGAllocProxyRes(byte[] buf) {
		PMGAllocProxyRes res = new PMGAllocProxyRes();
		res.unmarshall(buf);
		
		ProtoLog.log("CallProtoHandler.onMGAllocProxyRes, ip/port=" + res.ip + "," + res.port);
		//
		// hardcode ip for test purpose
		//mCallMgr.loginProxy(res.ip, res.port);
		//
		mCallMgr.loginProxy(res.ip, res.port);
	}
}
