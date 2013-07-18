package com.qihoo.mediasdk.call.proto;

import com.qihoo.mediasdk.protobase.ProtoPacket;

public class PMCAllocRes extends ProtoPacket {
	public short	res = 0;
	public String	ip = null;
	public short	port = 0;
	public int		isp1 = 0;
	public int		area1 = 0;
	public int		isp2 = 0;
	public int		area2 = 0;
	
	@Override
	public void unmarshall(byte[] buf) {
		super.unmarshall(buf);
		
		res = popShort();
		ip = popString16();
		port = popShort();
		isp1 = popInt();
		area1 = popInt();
		isp2 = popInt();
		area2 = popInt();
	}
}
