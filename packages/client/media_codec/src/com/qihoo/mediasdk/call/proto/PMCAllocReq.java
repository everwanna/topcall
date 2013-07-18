package com.qihoo.mediasdk.call.proto;

import com.qihoo.mediasdk.protobase.ProtoPacket;
import com.qihoo.mediasdk.protobase.ProtoSvid;
import com.qihoo.mediasdk.protobase.ProtoUris;

public class PMCAllocReq extends ProtoPacket {
	public int	isp1 = 0;
	public int 	area1 = 0;
	public int	isp2 = 0;
	public int 	area2 = 0;
	
	@Override
	public byte[] marshall()
	{		
		setSvid( ProtoSvid.SVID_MCENTER );
		setUri( ProtoUris.URI_MCENTER_ALLOC_REQ );
		
		pushInt(this.isp1);
		pushInt(this.area1);
		pushInt(this.isp2);
		pushInt(this.area2);		
		
		return super.marshall();
	}
}
