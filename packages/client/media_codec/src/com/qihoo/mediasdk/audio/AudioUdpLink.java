package com.qihoo.mediasdk.audio;

import com.qihoo.mediasdk.audio.proto.PMPPing;
import com.qihoo.mediasdk.protobase.IProtoLinkHandler;
import com.qihoo.mediasdk.protobase.ProtoLink;

public class AudioUdpLink implements IProtoLinkHandler {
	private AudioMgr mAudioMgr = null;
	private ProtoLink mLink = null;
	
	public AudioUdpLink(AudioMgr mgr) {
		mAudioMgr = mgr;
		mLink = new ProtoLink(false, this);		
	}
	
	public void connect(String ip, int port) {
		mLink.connect(ip, port);
	}
	
	public void close() {
		mLink.close();
	}
	
	public void send(byte[] buf) {
		send(buf, 0, buf.length);
	}
	
	public void send(byte[] buf, int offset, int len) {
		mLink.send(buf, offset, len);
	}

	@Override
	public void onConnected() {
		mAudioMgr.join("stream1");
		
		//start the heartbeat:
		mLink.addTimer(0, 1000);
	}

	@Override
	public void onData(short svid, short uri, byte[] buf) {
		mAudioMgr.getProtoHandler().onProto(svid, uri, buf);
	}

	@Override
	public void onDisconnected() {
		mLink.removeTimer(0);
	}

	@Override
	public void onTimer(int id) {
		PMPPing ping = new PMPPing(mAudioMgr.getSid(), mAudioMgr.getUid());
		send( ping.marshall());
	}
}
