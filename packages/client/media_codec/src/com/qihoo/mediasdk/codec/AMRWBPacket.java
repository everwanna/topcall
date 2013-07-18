package com.qihoo.mediasdk.codec;

public class AMRWBPacket {
	public AMRWBPacket() {
		
	}
	
	public static int getPT(byte b) {
		return (b>>3)&0x0F;
	}
}
