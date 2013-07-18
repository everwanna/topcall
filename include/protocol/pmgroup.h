#ifndef _PMGROUP_H
#define _PMGROUP_H

#include "core/packet.h"

#define URI_MGROUP_REPORT_REQ			100
#define URI_MGROUP_REPORT_RES			101
#define URI_MGROUP_ALLOC_REQ			102
#define URI_MGROUP_ALLOC_RES			103
#define URI_MGROUP_PREALLOC_REQ			105
#define URI_MGROUP_PREALLOC_RES			105
#define URI_MGROUP_PING					106

struct PMGReportReq : Packet {
	enum { uri=URI_MGROUP_REPORT_REQ };
	
	std::string		name;
	std::string		ip;
	short			port;
	short			udp;
	int				load;
	int				load_max;
	
	virtual void	unmarshall(Unpack& up) {
		name = up.popString();
		ip = up.popString();
		port = up.popInt16();
		udp = up.popInt16();
		load = up.popInt32();
		load_max = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(name);
		pk.pushString(ip);
		pk.pushInt16(port);
		pk.pushInt16(udp);
		pk.pushInt32(load);
		pk.pushInt32(load_max);
	}
};

struct PMGReportRes : Packet {
	enum { uri=URI_MGROUP_REPORT_RES };
	
	std::string name;
	int			res;
	
	virtual void	unmarshall(Unpack& up) {
		name = up.popString();
		res = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(name);
		pk.pushInt32(res);
	}
};


struct PMGAllocReq : Packet {
	enum { uri=URI_MGROUP_ALLOC_REQ };	
	
	int		uid;
	std::string stream;
	
	virtual void	unmarshall(Unpack& up) {
		uid = up.popInt32();
		stream = up.popString();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushInt32(uid);
		pk.pushString(stream);
	}
};

struct PMGAllocRes : Packet {
	enum { uri=URI_MGROUP_ALLOC_RES };
	
	int			res;
	int			uid;
	std::string stream;
	std::string	cookie;	
	std::string	name;
	std::string	ip;
	short		port;
	short		udp;
	int			load;
	int			load_max;
	
	virtual void	unmarshall(Unpack& up) {
		res = up.popInt32();
		uid = up.popInt32();
		stream = up.popString();
		cookie = up.popString();		
		name = up.popString();
		ip = up.popString();
		port = up.popInt16();
		udp = up.popInt16();
		load = up.popInt32();
		load_max = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(res);
		pk.pushInt32(uid);
		pk.pushString(stream);
		pk.pushString(cookie);		
		pk.pushString(name);
		pk.pushString(ip);
		pk.pushInt16(port);
		pk.pushInt16(udp);
		pk.pushInt32(load);
		pk.pushInt32(load_max);
	}
};

struct PMGPing : Packet {
	enum { uri=URI_MGROUP_PING };	
	
	int			uid;
	std::string stream;
	
	virtual void	unmarshall(Unpack& up) {	
		uid = up.popInt32();
		stream = up.popString();
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushInt32(uid);
		pk.pushString(stream);
	}
};


#endif
