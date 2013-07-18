package com.qihoo.mediasdk.net;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.concurrent.locks.ReentrantLock;

import android.annotation.SuppressLint;

public class NetMgr {	
	private static NetMgr sInstance = null;
	private static int sLinkId = 1;
	
	private Selector mSelector = null;
	private NetThread mThread = null;	
	
	private ReentrantLock mLinkMapLocker = new ReentrantLock();
	@SuppressLint("UseSparseArrays")
	private HashMap<Integer, NetLinkBase> mLinkMap = new HashMap<Integer, NetLinkBase>();
	
	private NetQueue mNetQueue = null;
	private NetTimerMgr mTimerMgr = null;
	
	
	public static NetMgr getInstance() {
		if( sInstance == null )
			sInstance = new NetMgr();
		
		return sInstance;
	}
	
	public NetMgr() {
		try {
			//android 2.1 doesn't support IPv6 good.
			java.lang.System.setProperty("java.net.preferIPv4Stack", "true");
    		java.lang.System.setProperty("java.net.preferIPv6Addresses", "false");
    		
    		mNetQueue = new NetQueue();
    		mTimerMgr = new NetTimerMgr(this);
    		
			mSelector = Selector.open();	
			mThread = new NetThread(this);
			mThread.start();			
		} catch (IOException e) {
			NetLog.log("NetMgr::NetMgr, Selector open exception=" + e.getMessage());
		}
		
	}
	
	public int create(Boolean tcp, INetLinkHandler handler) {	
		NetLog.log("NetMgr.create, enter.");
		mLinkMapLocker.lock();
		int linkid = sLinkId++;
		NetLinkBase link = null;
		if( tcp ) {
			link = new NetTcpLink(this, linkid, mSelector, handler);
		} else {
			link = new NetUdpLink(this, linkid, mSelector, handler);
		}
		mLinkMap.put(linkid, link);		
		mLinkMapLocker.unlock();
		
		NetLog.log("NetMgr.create, linkid=" + linkid);
		
		return linkid;
	}
	
	public void connect(int linkid, String ip, int port) {
		mLinkMapLocker.lock();
		
		NetMsg msg = new NetMsg();
		msg.linkid = linkid;			
		msg.type = NetMsg.NET_CONNECT;
		msg.ip = ip;
		msg.port = port;
		mNetQueue.push(msg);
		NetLog.log("NetMgr::connect, linkid/ip=" + linkid + "," + ip);
		
		mLinkMapLocker.unlock();
	}
	
	public void send(int linkid, byte[] buf, int offset, int len) {
		mLinkMapLocker.lock(); 
		
		NetMsg msg = new NetMsg();
		msg.linkid = linkid;
		msg.type = NetMsg.NET_SEND;
		msg.buf = buf;
		msg.offset = offset;
		msg.len = len;
		mNetQueue.push(msg);

		mLinkMapLocker.unlock();
	}
	
	public void close(int linkid) {
		mLinkMapLocker.lock();
		
		NetMsg msg = new NetMsg();
		msg.type = NetMsg.NET_CLOSE;
		msg.linkid = linkid;
		mNetQueue.push(msg);
	
		mLinkMapLocker.unlock();
	}
	
	public void addTimer(int linkid, int id, int interval) {
		mLinkMapLocker.lock(); 
		mTimerMgr.addTimer(linkid, id, interval);		
		mLinkMapLocker.unlock();
	}
	
	public void removeTimer(int linkid) {
		mLinkMapLocker.lock(); 
		mTimerMgr.removeTimer(linkid);				
		mLinkMapLocker.unlock();
	}
	
	public void removeTimer(int linkid, int id) {
		mLinkMapLocker.lock(); 
		mTimerMgr.removeTimer(linkid, id);				
		mLinkMapLocker.unlock();
	}
	
	public void select() {
		if( mLinkMap.size() == 0 )
			return;

		int num = 0;		
		try{ 
			num = mSelector.select(20);

			if( num > 0 ) {	
				
				mLinkMapLocker.lock();
				
				Set<SelectionKey> keys = mSelector.selectedKeys();
				Iterator<SelectionKey> it = keys.iterator();
				while( it.hasNext() ) {
					SelectionKey key = it.next();				
					
					NetLinkBase link = (NetLinkBase)key.attachment();
					if( link == null ) {
						NetLog.log("NetMgr::select, link==null.");
						continue;
					}
					
					//SocketChannel channel = link.getChannel();
					if( key.isReadable() ) {
						link.onRead();					
					} else if( key.isConnectable() ) {
						if( link.getClass() == NetTcpLink.class ) {
							NetTcpLink tcplink = (NetTcpLink)link;
							tcplink.finishConnect();
						}
						link.onConnected();
					}	
					
					it.remove();				
				}
				keys.clear();
				
				mLinkMapLocker.unlock();
			}
			
						
		} catch (IOException e) {
			NetLog.log("NetMgr::select, exception=" + e.getMessage());
		}	
	}
	
	public void getMsgs(List<NetMsg> msgs) {
		mNetQueue.fetch(msgs, 10);
	}
	
	public NetTimerMgr getTimerMgr() {
		return mTimerMgr;
	}
	
	//helper functions:
	public void sendDirect(int linkid, byte[] buf, int offset, int len) {
		mLinkMapLocker.lock();
		{
			NetLinkBase link = null;
			if( !mLinkMap.containsKey(linkid) ) {
				NetLog.log("NetMgr.sendDirect, fail to find link=" + linkid);
				return;
			}			
			link = mLinkMap.get(linkid);		
			if( link == null || link.getLinkId() != linkid ) {
				NetLog.log("NetMgr.sendDirect, link==null, linkid=" + linkid);
				return;
			}
			link.send( buf, offset, len );
		}
		
		mLinkMapLocker.unlock();
	}
	
	public void connectDirect(int linkid, String ip, int port) {
		NetLog.log("NetMgr.connectDirect, linkid/ip/port=" + linkid + "," + ip + "," + port);
		mLinkMapLocker.lock();
		{
			NetLinkBase link = null;		
			if( !mLinkMap.containsKey(linkid) ) {
				NetLog.log("NetMgr.connectDirect, invalid linkid=" + linkid);
				return;
			}
			
			link = mLinkMap.get(linkid);
			if( link == null || link.getLinkId() != linkid ) {
				NetLog.log("NetMgr.connectDirect, link==null, linkid=" + linkid);
				return;
			}
			link.connect(ip, port);		
		}
		mLinkMapLocker.unlock();
	}
	
	public void closeDirect(int linkid) {
		NetLog.log("NetMgr.closeDirect, linkid=" + linkid);
		mLinkMapLocker.lock();
		{
			NetLinkBase link = null;		
			if( !mLinkMap.containsKey(linkid) ) {
				NetLog.log("NetMgr.closeDirect, fail to find linkid=" + linkid);
				return;
			}
			link = mLinkMap.get(linkid);
			if( link == null || link.getLinkId() != linkid ) {
				NetLog.log("NetMgr.closeDirect, link==null, linkid=" + linkid);
				return;
			}
			
			link.close();
			mLinkMap.remove(linkid);
		}
		mLinkMapLocker.unlock();
	}
	
	public void timerDirect(int linkid, int id) {
		mLinkMapLocker.lock();
		{
			NetLinkBase link = null;		
			if( !mLinkMap.containsKey(linkid) ) {
				NetLog.log("NetMgr.timerDirect, fail to find linkid=" + linkid);
				mTimerMgr.removeTimer(linkid);
				return;
			}
			link = mLinkMap.get(linkid);
			if( link == null || link.getLinkId() != linkid ) {
				NetLog.log("NetMgr.timerDirect, link==null, linkid=" + linkid);
				return;
			}
			
			link.onTimer(id);			
		}
		mLinkMapLocker.unlock();
	}
}
