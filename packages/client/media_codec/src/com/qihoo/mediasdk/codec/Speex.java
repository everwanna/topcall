package com.qihoo.mediasdk.codec;

public class Speex {
	private static boolean s_loaded = false;
	
	public Speex() {
		if( !s_loaded ) {
			System.loadLibrary("speex");
			s_loaded = true;
		}
	}
	
	public native int	open(int sample_rate, int quality);
	public native int 	encode(byte[] src, int size, byte[] dest, int out_size);
	public native int 	decode(byte[] src, int size, byte[] dest, int out_size);
	public native void	close();
}
