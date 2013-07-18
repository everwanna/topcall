package com.qihoo.mediasdk.call.proto;

import com.qihoo.mediasdk.protobase.ProtoPacket;

public class PMGAllocProxyRes extends ProtoPacket {
	public int res = 0;
	public String stream = null;
	public String cookie = null;	
	public String name = null;
	public String ip = null;
	public short port = 0;
	public short udp = 0;
	public int load = 0;
	public int load_max = 0;
	
	@Override
	public void unmarshall(byte[] buf) {
		super.unmarshall(buf);
		res = popInt();
		stream = popString16();
		cookie = popString16();		
		name = popString16();
		ip = popString16();
		port = popShort();
		udp = popShort();
		load = popInt();
		load_max = popInt();
	}
}
