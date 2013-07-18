package com.qihoo.mediasdk.call.proto;

import com.qihoo.mediasdk.protobase.ProtoPacket;
import com.qihoo.mediasdk.protobase.ProtoSvid;
import com.qihoo.mediasdk.protobase.ProtoUris;

public class PMGAllocProxyReq extends ProtoPacket {
	public String stream = null;
	
	@Override
	public byte[] marshall()
	{		
		setSvid( ProtoSvid.SVID_MGROUP );
		setUri( ProtoUris.URI_MGROUP_ALLOC_PROXY_REQ );
		
		pushString16(stream);	
		
		return super.marshall();
	}
}
