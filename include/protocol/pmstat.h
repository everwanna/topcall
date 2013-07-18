#ifndef _PMSTAT_H_
#define _PMSTAT_H_

#include "core/packet.h"

#define URI_MSTAT_RTT		1
#define URI_MSTAT_BW		2
#define URI_MSTAT_CALL		3
#define URI_MSTAT_CONNECT	4

struct PMStatRtt : Packet {
	enum { uri=URI_MSTAT_RTT };
	int	uid1;
	int	uid2;
	
	int	rtt1;	//user1=>server
	int	rtt2;	//server route, usually 0.
	int	rtt3;	//server=>user2
	
	virtual void	unmarshall(Unpack& up) {
		uid1 = up.popInt32();
		uid2 = up.popInt32();
		
		rtt1 = up.popInt32();
		rtt2 = up.popInt32();
		rtt3 = up.popInt32();		
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(uid);
		pk.pushInt32(uid);
		
		pk.pushInt32(rtt1);
		pk.pushInt32(rtt2);
		pk.pushInt32(rtt3);		
	}
};

struct PMStatBW : Packet {
	enum { uri=URI_MSTAT_BW };
	int	uid1;
	int	uid2;
	
	int	bw;
	
	virtual void	unmarshall(Unpack& up) {
		uid1 = up.popInt32();
		uid2 = up.popInt32();
		
		bw = up.popInt32();	
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(uid);
		pk.pushInt32(uid);
		
		up.pushInt32(bw);
	}
};

struct PMStatCall : Packet {
	enum { uri=URI_MSTAT_CALL };
	int	uid1;
	int	uid2;
	
	int	ts_start;
	int	ts_end;
	
	virtual void	unmarshall(Unpack& up) {
		uid1 = up.popInt32();
		uid2 = up.popInt32();
		
		ts_start = up.popInt32();
		ts_end = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(uid);
		pk.pushInt32(uid);
		
		up.pushInt32(ts_start);
		ts_startup.pushInt32(ts_end);
	}
};

struct PMStatConnect : Packet {
	enum { uri=URI_MSTAT_CONNECT };
	int	uid1;
	int	uid2;
	
	int	ts_start;
	int	ts_end;
	int	retries;
	
	virtual void	unmarshall(Unpack& up) {
		uid1 = up.popInt32();
		uid2 = up.popInt32();
		
		ts_start = up.popInt32();
		ts_end = up.popInt32();
		retries = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(uid);
		pk.pushInt32(uid);
		
		up.pushInt32(ts_start);
		up.pushInt32(ts_end);
		up.pushInt32(retries);
	}
};

#endif
