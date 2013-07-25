package com.topcall.andsdk.login.proto;

import com.topcall.andsdk.protobase.ProtoPacket;
import com.topcall.andsdk.protobase.ProtoUris;

public class PLoginReq extends ProtoPacket {
	public int uid = 0;
	public String passport = null;
	public String password = null;
	
	@Override
	public byte[] marshall()
	{				
		setUri( ProtoUris.URI_LOGIN_REQ );
		
		pushInt(uid);
		pushString16(passport);
		pushString16(password);
		
		return super.marshall();
	}
}
