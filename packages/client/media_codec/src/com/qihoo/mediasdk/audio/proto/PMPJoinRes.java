package com.qihoo.mediasdk.audio.proto;

import com.qihoo.mediasdk.protobase.ProtoPacket;

public class PMPJoinRes extends ProtoPacket {
	public int res = 0;
	public int sid = 0;
	public int uid = 0;
	
	@Override
	public void unmarshall(byte[] buf) {
		super.unmarshall(buf);
		
		res = popInt();
		sid = popInt();
		uid = popInt();
	}
}
