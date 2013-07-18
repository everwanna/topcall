package com.qihoo.mediasdk.audio.record;

public class AMRWBConfig {
	public static final int MODE_0 = 6600;
	public static final int MODE_1 = 8850;
	public static final int MODE_2 = 12650;
	public static final int MODE_3 = 14250;
	public static final int MODE_4 = 15850;
	public static final int MODE_5 = 18250;
	public static final int MODE_6 = 19850;
	public static final int MODE_7 = 23050;
	public static final int MODE_8 = 23850;
	
	public static final int FRAME_SIZE_MODE_0 = 18;
	public static final int FRAME_SIZE_MODE_1 = 24;
	public static final int FRAME_SIZE_MODE_2 = 33;
	public static final int FRAME_SIZE_MODE_3 = 37;
	public static final int FRAME_SIZE_MODE_4 = 41;
	public static final int FRAME_SIZE_MODE_5 = 47;
	public static final int FRAME_SIZE_MODE_6 = 51;
	public static final int FRAME_SIZE_MODE_7 = 59;
	public static final int FRAME_SIZE_MODE_8 = 61;
	public static final int FRAME_SIZE_MODE_9 = 6;
	public static final int FRAME_SIZE_MODE_10 = 6;
	public static final int FRAME_SIZE_MODE_11 = 0;
	public static final int FRAME_SIZE_MODE_12 = 0;
	public static final int FRAME_SIZE_MODE_13 = 0;
	public static final int FRAME_SIZE_MODE_14 = 1;
	public static final int FRAME_SIZE_MODE_15 = 1;
	
	public static final int FILE_HEADER_SIZE = 9;
	public static final int SAMPLE_RATE = 16000;
	
	
	public static final int BYTE_0_MODE_0 = 0x04;
	public static final int BYTE_0_MODE_1 = 0x0C;
	public static final int BYTE_0_MODE_2 = 0x14;
	public static final int BYTE_0_MODE_3 = 0x1C;
	public static final int BYTE_0_MODE_4 = 0x24;
	public static final int BYTE_0_MODE_5 = 0x2C;
	public static final int BYTE_0_MODE_6 = 0x34;
	public static final int BYTE_0_MODE_7 = 0x3C;
	
	private static final int[] bit_rates = {MODE_0, MODE_1, MODE_2, MODE_3, MODE_4, MODE_5, MODE_6, MODE_7};
	private static final int[] frame_sizes = {
		FRAME_SIZE_MODE_0, 
		FRAME_SIZE_MODE_1, 
		FRAME_SIZE_MODE_2, 
		FRAME_SIZE_MODE_3,
		FRAME_SIZE_MODE_4, 
		FRAME_SIZE_MODE_5, 
		FRAME_SIZE_MODE_6, 
		FRAME_SIZE_MODE_7,
		FRAME_SIZE_MODE_8,
		FRAME_SIZE_MODE_9,
		FRAME_SIZE_MODE_10,
		FRAME_SIZE_MODE_11,
		FRAME_SIZE_MODE_12,
		FRAME_SIZE_MODE_13,
		FRAME_SIZE_MODE_14,
		FRAME_SIZE_MODE_15,
		};
	private static final int[] byte0s = {BYTE_0_MODE_0, BYTE_0_MODE_1, BYTE_0_MODE_2, BYTE_0_MODE_3,
		BYTE_0_MODE_4, BYTE_0_MODE_5, BYTE_0_MODE_6, BYTE_0_MODE_7};
	
	private int mSeq = 0;
	public AMRWBConfig(int seq) {
		mSeq = seq;
	}
	
	public int	getBitRate() {
		return bit_rates[mSeq];
	}
	
	public int getFrameSize() {
		return frame_sizes[mSeq];
	}
	
	public int getByte0() {
		return byte0s[mSeq];
	}
	
	public int getHeaderSize() {
		return FILE_HEADER_SIZE;
	}
	
	public static int getFrameSize(int seq) {
		return frame_sizes[seq];
	}
}
