package com.qihoo.mediasdk.net;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;

public class NetUdpLink extends NetLinkBase {
	protected DatagramChannel mChannel = null;
	protected ByteBuffer mReadBuffer = null;
	protected ByteBuffer mSendBuffer = null;
	
	public NetUdpLink(NetMgr mgr, int linkid, Selector selector, INetLinkHandler handler) {
		super(mgr, linkid, selector, handler);
		
		try {
			mChannel = DatagramChannel.open();
			mChannel.configureBlocking(false);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		this.mReadBuffer = ByteBuffer.allocate(PROTO_UDP_BUFFER_SIZE);
		this.mReadBuffer.order(ByteOrder.LITTLE_ENDIAN);
		this.mSendBuffer = ByteBuffer.allocate(PROTO_UDP_BUFFER_SIZE);
		this.mSendBuffer.order(ByteOrder.LITTLE_ENDIAN);
	}

	@Override
	public void connect(String ip, int port) {
		NetLog.log("NetUdpLink.connect, ip/port=" + ip + "," + port);
		
		try {
			InetSocketAddress addr = new InetSocketAddress( InetAddress.getByName(ip), port);
			mChannel.register(mSelector, SelectionKey.OP_READ );
			mChannel.keyFor(mSelector).attach(this);
			//mChannel.socket().setReuseAddress(true);
			//mChannel.socket().bind( new InetSocketAddress(PROTO_UDP_PORT) );			
			mChannel.connect(addr);		
			
			onConnected();
		} catch (IOException e) {
			NetLog.log("NetUdpLink.connect, exception=" + e.getMessage());
		}	
	}
	
	@Override
	public void	send(byte[] buf, int offset, int len) {
		try {
			mChannel.write( ByteBuffer.wrap(buf, offset, len) );
		} catch (IOException e) {
			NetLog.log("NetUdpLink.send, exception=" + e.getMessage());
		}
	}
	
	@Override
	public void close() {
		NetLog.log("NetUdpLink.close, linkid=" + mLinkId);
		try {			
			mChannel.close();
		} catch (IOException e) {
			NetLog.log("NetUdpLink.close, exception=" + e.getMessage());
		}
	}
	
	@Override
	public void onRead() {
		if( !mChannel.isConnected() ) {
			NetLog.log("NetUdpLink.onRead, mChannel is not connected.");
			return;
		}
		try {		
			mReadBuffer.clear();
			int len = mChannel.read(mReadBuffer);
			//ProtoLog.log("NetLink.onRead, linkid/len=" + mLinkId + "," + len);	
			
			if( len == -1 ) {
				NetLog.log("NetUdpLink.onRead, len == -1");
				close();
				return;
			}
			mReadBuffer.flip();
			mHandler.onData(mReadBuffer);
		} catch (IOException e) {
			NetLog.log("NetUdpLink.onRead, exception=" + e.getMessage());
			close();
			
			mHandler.onDisconnected();
		}
	}
	
	@Override
	public void onConnected() {
		NetLog.log("NetUdpLink.onConnected, linkid=" + mLinkId);
		mHandler.onConnected();
	}
}
