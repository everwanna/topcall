package com.qihoo.mediasdk.protobase;


public interface IProtoLinkHandler
{
	void onConnected();	
	void onData(short svid, short uri, byte[] buf);
	void onDisconnected();
	void onTimer(int id);
}