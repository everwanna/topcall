package com.qihoo.mediasdk.protobase;

public class ProtoLog {
	public static final int LOG_AUDIO = 0;	
	private static int s_ctl_audio = 0;
	
	public static void log(String msg) {
		long threadid = Thread.currentThread().getId();
		System.out.println("MEDIA - " + threadid + " - " + msg);
	}
	
	public static void error(String msg) {
		long threadid = Thread.currentThread().getId();
		System.out.println("MEDIA - ERROR - " + threadid + " - " + msg);
	}
	
	public static void logctl(String msg, int tag) {
		switch(tag) {
		case LOG_AUDIO:
			if( s_ctl_audio>=1000 ) {
				log(msg);
				s_ctl_audio = 0;
			}
			break;
		}
	}
}
