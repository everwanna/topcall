package com.qihoo.mediasdk.protobase;

import java.nio.ByteBuffer;

public interface IProtoPacket {
	public byte[] marshall();
	public void marshall(ByteBuffer buf);
	public void unmarshall(byte[] buf);
	public void unmarshall(ByteBuffer buf);
	public void clear();
}
