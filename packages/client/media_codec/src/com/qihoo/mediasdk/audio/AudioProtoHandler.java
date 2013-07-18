package com.qihoo.mediasdk.audio;

import com.qihoo.mediasdk.audio.proto.PMPStreamData;
import com.qihoo.mediasdk.audio.proto.PMPJoinRes;
import com.qihoo.mediasdk.call.proto.PMCAllocRes;
import com.qihoo.mediasdk.protobase.ProtoLog;
import com.qihoo.mediasdk.protobase.ProtoSvid;
import com.qihoo.mediasdk.protobase.ProtoUris;

public class AudioProtoHandler {
	private AudioMgr mAudioMgr = null;
	
	public AudioProtoHandler(AudioMgr mgr) {
		this.mAudioMgr = mgr;
	}
	
	public void onProto(short svid, short uri, byte[] buf) {
		if( svid == ProtoSvid.SVID_MPROXY ) {
			switch(uri) {
			case ProtoUris.URI_MPROXY_STREAM_DATA:
				onStreamData(buf);
				break;
			case ProtoUris.URI_MPROXY_JOIN_RES:
				onJoinRes(buf);
				break;
			case ProtoUris.URI_MPROXY_LEAVE_RES:
				onLeaveRes(buf);
				break;
			
			}	
		}
	}

	/**
	 * [TBD] memory pool need to be added here.
	 * @param buf
	 */
	private void onStreamData(byte[] buf) {
		PMPStreamData res = new PMPStreamData();
		res.unmarshall(buf);
		
		//ProtoLog.log("AudioProtoHandler.onStreamData, size=" + res.payload.length);
		mAudioMgr.getPlayer().play(res.payload);
	}

	
	private void onJoinRes(byte[] buf) {		
		PMPJoinRes res = new PMPJoinRes();
		res.unmarshall(buf);
		
		mAudioMgr.setSid(res.sid);
		
		ProtoLog.log("AudioProtoHandler.onJoinRes, sid/uid=" + res.sid + res.uid);
		mAudioMgr.getRecorder().startRecord();
	}
	
	private void onLeaveRes(byte[] buf) {		
		mAudioMgr.getRecorder().stopRecord();
	}
}
