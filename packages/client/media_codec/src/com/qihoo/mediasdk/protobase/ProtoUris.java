package com.qihoo.mediasdk.protobase;

public class ProtoUris {
	//mproxy
	public static final short URI_MPROXY_JOIN_REQ = 5;
	public static final short URI_MPROXY_JOIN_RES = 6;
	public static final short URI_MPROXY_LEAVE_REQ = 7;
	public static final short URI_MPROXY_LEAVE_RES = 8;	
	public static final short URI_MPROXY_STREAM_EVENT	= 9;
	public static final short URI_MPROXY_PING			= 10;
	
	public static final short URI_MPROXY_STREAM_DATA	= 100;
	public static final short URI_MPROXY_STREAM_DATA2	= 101;
	public static final short URI_MPROXY_RESEND_REQ = 102;
	
	//mcenter&mcproxy
	public static final short URI_MCENTER_REPORT_REQ = 1;
	public static final short URI_MCENTER_ALLOC_REQ = 100;
	public static final short URI_MCENTER_ALLOC_RES = 101;
	public static final short URI_MCENTER_PREALLOC_REQ = 200;
	public static final short URI_MCENTER_PREALLOC_RES = 201;
	
	//mgroup:
	public static final short URI_MGROUP_ALLOC_PROXY_REQ = 102;
	public static final short URI_MGROUP_ALLOC_PROXY_RES = 103;
}

