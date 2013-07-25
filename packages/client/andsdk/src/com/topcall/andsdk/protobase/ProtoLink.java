package com.topcall.andsdk.protobase;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import com.topcall.andsdk.net.INetLinkHandler;
import com.topcall.andsdk.net.NetMgr;


public class ProtoLink implements INetLinkHandler {
	private int mLinkId = 0;
	private IProtoLinkHandler mHandler = null;
	private ByteBuffer mRecvBuffer = null;
	private int mStart = 0;
	private int mLen = 0;
	private Boolean mConnected = false;
	private int mCapacity = 0;
	
	public ProtoLink(Boolean tcp, IProtoLinkHandler handler) {
		mHandler = handler;
		mLinkId = NetMgr.getInstance().create(tcp, this);
		
		if( tcp ) { 
			mCapacity = ProtoConst.PROTO_TCP_BUFFER_SIZE;			
		}
		else {
			mCapacity = ProtoConst.PROTO_UDP_BUFFER_SIZE;			
		}
		mRecvBuffer = ByteBuffer.allocate(mCapacity);
		//mRecvBuffer.order(ByteOrder.LITTLE_ENDIAN);
	}
	
	public void connect(String ip, int port) {
		ProtoLog.log("ProtoLink.connect, linkid/ip/port=" + mLinkId + "," + ip + "," + port);
		NetMgr.getInstance().connect(mLinkId, ip, port);
	}
	
	public void close() {
		NetMgr.getInstance().removeTimer(mLinkId);
		NetMgr.getInstance().close(mLinkId);
		mConnected = false;
	}
	
	public void send(byte[] buf, int offset, int len) {		
		NetMgr.getInstance().send(mLinkId, buf, offset, len);
	}
	
	public void addTimer(int id, int interval) {
		NetMgr.getInstance().addTimer(mLinkId, id, interval);
	}
	
	public void removeTimer(int id) {
		NetMgr.getInstance().removeTimer(mLinkId);
	}
	
	public int getLinkId() {
		return mLinkId;
	}
	
	public Boolean isConnected() {
		return mConnected;
	}

	@Override
	public void onConnected() {
		mHandler.onConnected();
		mConnected = true;
	}

	@Override
	public void onData(ByteBuffer buffer) {
		//ProtoLog.log("ProtoLink.onData, size=" + buffer.remaining());
		
		//maybe a huge packet comes, we need large recv buffer:
		//if( mDataSplite ) {
		//	ProtoLog.log("ProtoLink.onData, be careful.");
		//}
		int new_len = buffer.remaining();
		if( mRecvBuffer.capacity() - mStart - mLen < buffer.limit() ) {
			shuffer( mRecvBuffer.capacity()*2 );
		}		
		mRecvBuffer.position(mStart+mLen);
		mRecvBuffer.put(buffer);		
		mRecvBuffer.position(mStart);
		mLen += new_len;
		
		while(true) {						
			if( mLen<=10 ) {
				return;
			}			
			int len = mRecvBuffer.getInt();
			int uri = mRecvBuffer.getInt();
			//int res = mRecvBuffer.getShort();
			//ProtoLog.log("ProtoLink.onData, len=" + len);
			if( len > 1024*1024*8 || len < 0 || len == 0 ) {
				ProtoLog.log("ProtoLink.onData, len>8M!!!, mLinkId/len=" + mLinkId + "," + len);
				mRecvBuffer.clear();
				mLen = 0;
				mStart = 0;
				return;
			}
			
			if( len > mLen ) {
				ProtoLog.log("ProtoLink.onData, not enough data, linkid/len//uri/remain=" + mLinkId + "," +  + len + "," + uri + "," + mLen);				
				mRecvBuffer.position(mStart);
				shuffer( Math.max(len, mCapacity) );				
				return;
			}				
			mRecvBuffer.position(mStart);
			
			byte[] buf = new byte[len];
			mRecvBuffer.get(buf);
			
			mHandler.onData(uri, buf);
			mStart += len;
			mLen -= len;
			if( mLen == 0 ) {
				mRecvBuffer.clear();
				mStart = 0;	
			} else if( mStart > 10000 ) {
				shuffer( Math.max(mLen, mCapacity) );
			}
			
			//mDataSplite = false;
		}
	}

	@Override
	public void onDisconnected() {
		mConnected = false;
		mHandler.onDisconnected();		
	}

	@Override
	public void onTimer(int id) {
		mHandler.onTimer(id);
	}

	private void shuffer(int size) {
		ByteBuffer buf = ByteBuffer.allocate(size);
		buf.order(ByteOrder.LITTLE_ENDIAN);
		buf.limit(size);
		
		buf.put(mRecvBuffer.array(), mStart, mLen);
		buf.position(0);
		mStart = 0;
		mRecvBuffer = buf;
		mCapacity = size;
	}
}
