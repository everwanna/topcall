package com.qihoo.mediasdk.audio;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

public class AudioPlayer {
	private AudioMgr mAudioMgr = null;
	private AudioTrack mTrack = null;
	private byte[] mBuffer = new byte[2048];
	
	public AudioPlayer(AudioMgr mgr) {
		mAudioMgr = mgr;
		startPlay();
	}

	public void startPlay() {
		int bufsize = AudioTrack.getMinBufferSize(16000, 
		AudioFormat.CHANNEL_CONFIGURATION_MONO,
		AudioFormat.ENCODING_PCM_16BIT);

		mTrack = new AudioTrack(AudioManager.STREAM_MUSIC, 
				16000,
				AudioFormat.CHANNEL_CONFIGURATION_MONO,
				AudioFormat.ENCODING_PCM_16BIT,
				bufsize,
				AudioTrack.MODE_STREAM);

		mTrack.play();
	}
	
	public void play(byte[] buf) {
		int len = mAudioMgr.getSilk().decode(buf, buf.length, mBuffer, 2048);
		if( len > 0 ) {
			mTrack.write(mBuffer, 0,  len);
		}
	}
}
