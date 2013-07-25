package com.topcall.andsdk.net;

import java.util.Iterator;
import java.util.LinkedList;

public class NetTimerMgr {
	private NetMgr mNetMgr = null;
	private LinkedList<NetTimer> mTimerList = new LinkedList<NetTimer>();
	
	public NetTimerMgr(NetMgr mgr) {
		this.mNetMgr = mgr;
	}
	
	public void	addTimer(int linkid, int id, int interval) {
		//if the timer exist, just exit:
		for( int i=0; i<mTimerList.size(); i++ ) {
			NetTimer timer = mTimerList.get(i);
			if( timer != null && timer.id == id && timer.linkid == linkid ) {
				NetLog.log("NetTimerMgr.addTimer, timer exist for linkid/id=" + linkid + "," + id);
				return;
			}
		}
		
		NetTimer timer = new NetTimer();
		timer.linkid = linkid;
		timer.id = id;
		timer.interval = interval;
		timer.last = System.currentTimeMillis();
		
		mTimerList.add(timer);
	}
	
	public void removeTimer(int linkid) {
		Iterator<NetTimer> it = mTimerList.iterator();
		while(it.hasNext()) {
			NetTimer timer = it.next();
			if(timer.linkid == linkid ) {
				it.remove();
			}
		}
	}
	
	public void removeTimer(int linkid, int id) {
		Iterator<NetTimer> it = mTimerList.iterator();
		while(it.hasNext()) {
			NetTimer timer = it.next();
			if(timer.linkid == linkid && timer.id == id ) {
				it.remove();
			}
		}
	}
	
	public void check() {
		long now = System.currentTimeMillis();
		Iterator<NetTimer> it = mTimerList.iterator();
		while(it.hasNext()) {
			NetTimer timer = it.next();
			if( timer.last + timer.interval <= now ) {
				mNetMgr.timerDirect(timer.linkid, timer.id);
				timer.last = now;
			}
		}
	}
}
