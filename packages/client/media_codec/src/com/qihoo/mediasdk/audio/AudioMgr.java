package com.qihoo.mediasdk.audio;

import com.qihoo.mediasdk.audio.proto.PMPStreamData;
import com.qihoo.mediasdk.audio.proto.PMPJoinReq;
import com.qihoo.mediasdk.audio.record.IRecordListener;
import com.qihoo.mediasdk.audio.record.IRecordMgr;
import com.qihoo.mediasdk.audio.record.amr.AMRRecordMgr;
import com.qihoo.mediasdk.audio.record.spx.SpxRecordMgr;
import com.qihoo.mediasdk.codec.AMRWBDecoder;
import com.qihoo.mediasdk.codec.Silk;
import com.qihoo.mediasdk.codec.Speex;

public class AudioMgr implements IRecordListener {
	private int mSid = 0;
	private int mUid = 0;
	private AudioUdpLink mUdpLink = null;	
	private AudioProtoHandler mProtoHandler = null;	
	private IRecordMgr mRecordMgr = null;
	private AudioPlayer mPlayer = null;	
	private AudioWriter mWriter = null;	
	private AudioFilePlayer mFilePlayer = null;
	private Speex mSpeex = null;
	private Silk mSilk = null;
	
	public AudioMgr(int uid) {
		mUid = uid;	
				
		mProtoHandler = new AudioProtoHandler(this);
		mRecordMgr = new AMRRecordMgr(this);		
		//mPlayer = new AudioPlayer(this);
		//mWriter = new AudioWriter();
		mFilePlayer = new AudioFilePlayer();
		//mSpeex = new Speex();
		//mSpeex.open(8000, 1);
		
		//mSilk = new Silk();
		//mSilk.open(16000, 4);
	}
	
	public void connect(String ip, int port) {
		mUdpLink = new AudioUdpLink(this);
		mUdpLink.connect(ip , port);		
	}
	
	public void close() {
		mUdpLink.close();		
	}
	
	public void	join(String stream) {
		PMPJoinReq req = new PMPJoinReq(mUid, "nick2", stream);
		mUdpLink.send( req.marshall() );
	}
	
	public void	leave() {
		
	}
	
	public int getUid() {
		return mUid;
	}
	
	public void setSid(int sid) {
		mSid = sid;
	}
	
	public int getSid() {
		return mSid;
	}
	
	public AudioUdpLink getUdpLink() {
		return mUdpLink;
	}
	
	public AudioProtoHandler getProtoHandler() {
		return mProtoHandler;
	}
	
	public IRecordMgr getRecorder() {
		return mRecordMgr;
	}
	
	public AudioPlayer getPlayer() {
		return mPlayer;
	}
	
	public AudioWriter getWriter() {
		return mWriter;
	}
	
	public AudioFilePlayer getFilePlayer() {
		return mFilePlayer;
	}
	
	public Speex getSpeex() {
		return mSpeex;
	}
	
	public Silk getSilk() {
		return mSilk;
	}

	@Override
	public void onFrame(byte[] buf, int offset, int len) {		
		/*
		//send the packet to mproxy:
		PMPStreamData pkg = new PMPStreamData();
		pkg.sid = mSid;
		pkg.from = mUid;
		pkg.payload = buf;
		pkg.len = len;
		mUdpLink.send(pkg.marshall());
		*/
		
		mWriter.write(buf, offset, len);
	}
}
