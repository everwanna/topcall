package com.topcall.andsdk.net;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;

public class NetTcpLink extends NetLinkBase {
	protected SocketChannel mChannel = null;
	protected ByteBuffer mReadBuffer = null;
	
	public NetTcpLink(NetMgr mgr, int linkid, Selector selector, INetLinkHandler handler) {
		super(mgr, linkid, selector, handler);
		
		try {
			mChannel = SocketChannel.open();
			mChannel.configureBlocking(false);
		} catch (IOException e) {
			e.printStackTrace();
		}				 
		
		this.mReadBuffer = ByteBuffer.allocate(PROTO_TCP_BUFFER_SIZE);
	}

	@Override
	public void connect(String ip, int port) {
		NetLog.log("NetTcpLink.connect, ip/port=" + ip + "," + port);
		InetSocketAddress addr = new InetSocketAddress(ip, port);
		try {			
			mChannel.register(mSelector, SelectionKey.OP_CONNECT );
			mChannel.keyFor(mSelector).attach(this);
			mChannel.connect(addr);			
		} catch (IOException e) {
			NetLog.log("NetTcpLink.connect, exception=" + e.getMessage());
		}	
	}
	
	@Override
	public void	send(byte[] buf, int offset, int len) {
		try {
			mChannel.write( ByteBuffer.wrap(buf, offset, len) );
		} catch (IOException e) {
			NetLog.log("NetTcpLink.send, linkid/exception=" + mLinkId +"," + e.getMessage());
		}
	}
	
	@Override
	public void close() {
		NetLog.log("NetTcpLink.close, linkid=" + mLinkId);
		try {			
			mChannel.close();
		} catch (IOException e) {
			NetLog.log("NetTcpLink.close, linkid/exception=" + mLinkId + "," + e.getMessage());
		}
	}
	
	public void finishConnect() {
		if( mChannel.isConnectionPending() ) {
			try {
				mChannel.finishConnect();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	@Override
	public void onRead() {
		if( !mChannel.isConnected() ) {
			NetLog.log("NetLink.onRead, mChannel is not connected.");
			return;
		}
		try {		
			mReadBuffer.clear();
			int len = mChannel.read(mReadBuffer);
			//ProtoLog.log("NetLink.onRead, linkid/len=" + mLinkId + "," + len);	
			
			if( len == -1 ) {
				NetLog.log("NetLink.onRead, len == -1");
				close();
				return;
			}
			mReadBuffer.flip();
			mHandler.onData(mReadBuffer);
		} catch (IOException e) {
			NetLog.log("NetLink.onRead, exception=" + e.getMessage());
			close();
			
			mHandler.onDisconnected();
		}
	}
	
	@Override
	public void onConnected() {
		NetLog.log("NetLink.onConnected, linkid=" + mLinkId);
		try {			
			mChannel.register(mSelector, SelectionKey.OP_READ);
			mChannel.keyFor(mSelector).attach(this);
		} catch (ClosedChannelException e) {
			NetLog.log("NetLink.onConnected, exception=" + e.getMessage());
		}
		mHandler.onConnected();
	}
}
