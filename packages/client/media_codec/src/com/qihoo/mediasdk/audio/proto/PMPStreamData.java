package com.qihoo.mediasdk.audio.proto;

import com.qihoo.mediasdk.protobase.ProtoPacket;
import com.qihoo.mediasdk.protobase.ProtoSvid;
import com.qihoo.mediasdk.protobase.ProtoUris;

public class PMPStreamData extends ProtoPacket {
	public int sid = 0;
	public int from = 0;	
	public int seq = 0;
	public byte[] payload = null;
	public int len = 0;
	
	@Override
	public void unmarshall(byte[] buf) {
		super.unmarshall(buf);
		
		this.sid = popInt();
		this.from = popInt();
		this.seq = popInt();
		this.payload = popBytes();
	}
	
	public byte[] marshall()
	{		
		setSvid( ProtoSvid.SVID_MPROXY);
		setUri( ProtoUris.URI_MPROXY_STREAM_DATA );
		pushInt(sid);
		pushInt(from);
		pushInt(seq);
		pushBytes(payload, len);
		
		return super.marshall();
	}
}
