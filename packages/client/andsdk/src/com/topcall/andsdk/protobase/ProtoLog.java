package com.topcall.andsdk.protobase;

public class ProtoLog {
	public static void log(String msg) {
		long threadid = Thread.currentThread().getId();
		System.out.println("topcall - " + threadid + " - " + msg);
	}
	
	public static void error(String msg) {
		long threadid = Thread.currentThread().getId();
		System.out.println("topcall - ERROR - " + threadid + " - " + msg);
	}
}
