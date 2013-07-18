package com.qihoo.mediasdk.audio.proto;

import com.qihoo.mediasdk.protobase.ProtoPacket;
import com.qihoo.mediasdk.protobase.ProtoSvid;
import com.qihoo.mediasdk.protobase.ProtoUris;

public class PMPPing extends ProtoPacket {
	public int sid = 0;
	public int uid = 0;	
	
	public PMPPing(int sid, int uid) {
		this.sid = sid;
		this.uid = uid;		
	}
	
	@Override
	public byte[] marshall()
	{		
		setSvid(ProtoSvid.SVID_MPROXY);
		setUri(ProtoUris.URI_MPROXY_PING);	//[TBD]
		pushInt(sid);
		pushInt(uid);		
		
		return super.marshall();
	}
}
