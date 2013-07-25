package com.topcall.andsdk.net;

public class NetMsg {
	public static final int NET_UNKNOWN = 0;
	public static final int NET_CONNECTED = 1;
	public static final int NET_DATA = 2;
	public static final int NET_DISCONNECTED = 3;
	
	public static final int NET_SEND = 4;
	public static final int NET_CONNECT = 5;
	public static final int NET_CLOSE = 6;
	public static final int NET_TIMER = 7;
	
	public int linkid = 0;
	public int type = NET_UNKNOWN;
	public String ip = null;
	public int port = 0;
	public byte[] buf = null;
	public int offset = 0;
	public int len = 0;
}
