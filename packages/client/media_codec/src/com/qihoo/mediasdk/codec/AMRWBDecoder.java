package com.qihoo.mediasdk.codec;

public class AMRWBDecoder {
	private static boolean s_loaded = false;
	
	public AMRWBDecoder() {
		if( !s_loaded ) {
			System.loadLibrary("amrwb");
		}
	}
	
	public native int	open(int sample_rate, int channels);
	public native int 	decode(byte[] src, int size, byte[] dest, int out_size);
	public native void	close();
}
