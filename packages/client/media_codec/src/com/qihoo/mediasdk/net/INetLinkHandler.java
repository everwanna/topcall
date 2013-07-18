package com.qihoo.mediasdk.net;

import java.nio.ByteBuffer;

public interface INetLinkHandler {
	public void	onConnected();
	public void onData(ByteBuffer buffer);
	public void onDisconnected();
	public void onTimer(int id);
}
