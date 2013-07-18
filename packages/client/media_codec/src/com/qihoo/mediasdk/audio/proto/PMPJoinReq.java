package com.qihoo.mediasdk.audio.proto;

import com.qihoo.mediasdk.protobase.ProtoPacket;
import com.qihoo.mediasdk.protobase.ProtoSvid;
import com.qihoo.mediasdk.protobase.ProtoUris;

public class PMPJoinReq extends ProtoPacket {
	public int uid = 0;
	public String nick = null;
	public String stream = null;
	
	public  PMPJoinReq(int uid, String nick, String stream)
	{
		this.uid = uid;
		this.nick = nick;
		this.stream = stream;
	}
	
	public byte[] marshall()
	{		
		setSvid( ProtoSvid.SVID_MPROXY);
		setUri( ProtoUris.URI_MPROXY_JOIN_REQ );
		pushInt(this.uid);
		pushString16(this.nick);
		pushString16(this.stream);
		
		return super.marshall();
	}
}
