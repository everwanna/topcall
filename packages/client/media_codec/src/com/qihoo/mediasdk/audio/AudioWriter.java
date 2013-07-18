package com.qihoo.mediasdk.audio;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

import android.os.Environment;

public class AudioWriter {
	private String mFilename = null;	
	private FileOutputStream mFileStream = null;
	
	public AudioWriter() {
		mFilename = Environment.getExternalStorageDirectory().getAbsolutePath() + "/audio.amr";
		//mFilename = "/storage/sdcard0/audio.amr";
		try {
			mFileStream = new FileOutputStream(mFilename);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	public void	write(ByteBuffer bf){
		try {
			mFileStream.write(bf.array(), 0, bf.limit());
			mFileStream.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	} 
	
	public void write(byte[] buf, int offset, int len) {
		try {
			mFileStream.write(buf, offset, len);
			mFileStream.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}
	
	public void close() {
		try {
			mFileStream.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
