package com.qihoo.mediasdk.codec;

public class Silk {
	private static boolean s_loaded = false;
	
	public Silk() {
		if( !s_loaded ) {
			System.loadLibrary("silk");
			s_loaded = true;
		}
	}
	
	public native int	open(int sample_rate, int quality);
	public native int 	encode(byte[] src, int size, byte[] dest, int out_size);
	public native int 	decode(byte[] src, int size, byte[] dest, int out_size);
	public native void	close();
}
