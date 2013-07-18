package com.qihoo.mediasdk.audio;

import com.qihoo.mediasdk.protobase.ProtoLog;

import android.media.MediaPlayer;
import android.os.Environment;

public class AudioFilePlayer {
	private String mFilename = null;
	private MediaPlayer mPlayer = null;
	
	public AudioFilePlayer() {
		//mFilename = Environment.getExternalStorageDirectory().getAbsolutePath() + "/audio.amr";
		mFilename = "/mnt/sdcard/audio.amr";
	}
	
	public void	play() {
		mPlayer = new MediaPlayer();
		try {
			mPlayer.setDataSource(mFilename);
			mPlayer.prepare();
			mPlayer.start();
		} catch(Exception ex) {
			ProtoLog.error("AudioFilePlayer.play, exceptio=" + ex.getMessage());
		}
	}
}
