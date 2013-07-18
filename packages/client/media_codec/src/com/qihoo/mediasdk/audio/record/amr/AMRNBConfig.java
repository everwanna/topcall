package com.qihoo.mediasdk.audio.record.amr;

public class AMRNBConfig {
	public static final int MODE_0 = 4750;
	public static final int MODE_1 = 5150;
	public static final int MODE_2 = 5900;
	public static final int MODE_3 = 6700;
	public static final int MODE_4 = 7400;
	public static final int MODE_5 = 7950;
	public static final int MODE_6 = 1020;
	public static final int MODE_7 = 1220;
	
	public static final int FRAME_SIZE_MODE_0 = 13;
	public static final int FRAME_SIZE_MODE_1 = 14;
	public static final int FRAME_SIZE_MODE_2 = 16;
	public static final int FRAME_SIZE_MODE_3 = 18;
	public static final int FRAME_SIZE_MODE_4 = 20;
	public static final int FRAME_SIZE_MODE_5 = 21;
	public static final int FRAME_SIZE_MODE_6 = 27;
	public static final int FRAME_SIZE_MODE_7 = 32;
	
	public static final int FILE_HEADER_SIZE = 6;
	public static final int SAMPLE_RATE = 8000;
	
	public static final int BYTE_0_MODE_0 = 0x04;
	public static final int BYTE_0_MODE_1 = 0x0C;
	public static final int BYTE_0_MODE_2 = 0x14;
	public static final int BYTE_0_MODE_3 = 0x1C;
	public static final int BYTE_0_MODE_4 = 0x24;
	public static final int BYTE_0_MODE_5 = 0x2C;
	public static final int BYTE_0_MODE_6 = 0x34;
	public static final int BYTE_0_MODE_7 = 0x3C;
	
	private static final int[] bit_rates = {MODE_0, MODE_1, MODE_2, MODE_3, MODE_4, MODE_5, MODE_6, MODE_7};
	private static final int[] frame_sizes = {FRAME_SIZE_MODE_0, FRAME_SIZE_MODE_1, FRAME_SIZE_MODE_2, FRAME_SIZE_MODE_3,
		FRAME_SIZE_MODE_4, FRAME_SIZE_MODE_5, FRAME_SIZE_MODE_6, FRAME_SIZE_MODE_7};
	private static final int[] byte0s = {BYTE_0_MODE_0, BYTE_0_MODE_1, BYTE_0_MODE_2, BYTE_0_MODE_3,
		BYTE_0_MODE_4, BYTE_0_MODE_5, BYTE_0_MODE_6, BYTE_0_MODE_7};
	
	private int mSeq = 0;
	public AMRNBConfig(int seq) {
		mSeq = seq;
	}
	
	public int getHeaderSize() {
		return FILE_HEADER_SIZE;
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
	
	public static int getFrameSize(int seq) {
		return frame_sizes[seq];
	}
}
