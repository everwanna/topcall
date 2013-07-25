package com.topcall.andsdk.login.proto;

import com.topcall.andsdk.protobase.ProtoPacket;

public class PLoginRes extends ProtoPacket {
	public int	res = 0;
	public int	uid = 0;
	public String cookie = null;
	
	@Override
	public void unmarshall(byte[] buf)
	{		
		super.unmarshall(buf);
		res = popInt();
		uid = popInt();
		cookie = popString16();
	}
}
