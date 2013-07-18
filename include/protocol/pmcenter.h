#ifndef _PMCENTER_H
#define _PMCENTER_H

#include "core/packet.h"

#define URI_MCENTER_REPORT_REQ			1	//mgroup report ot mcenter.
#define URI_MCENTER_REPORT_RES			2	
#define URI_MCENTER_DELETE_GRP_REQ		3	//mgroup delete from mcenter.
#define URI_MCENTER_DELETE_GRP_RES		4

#define URI_MCENTER_PING				5

#define URI_MCENTER_ALLOC_REQ			100
#define URI_MCENTER_ALLOC_RES			101

//mcproxy:
#define URI_MCENTER_PREALLOC_REQ		200
#define URI_MCENTER_PREALLOC_RES		201

struct PMCReportReq : Packet {
	enum { uri=URI_MCENTER_REPORT_REQ };
	
	std::string	name;	//usually idc name.
	std::string ip;
	short	port;
	int		isp;
	int		area;	
	int		load_max;
	int		load;
	std::string	access_url;
	
	virtual void	unmarshall(Unpack& up) {
		name = up.popString();				
		ip = up.popString();	
		port = up.popInt16();
		isp = up.popInt32();
		area = up.popInt32();
		load = up.popInt32();
		load_max = up.popInt32();	
		access_url = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(name);	
		pk.pushString(ip);
		pk.pushInt16(port);
		pk.pushInt32(isp);
		pk.pushInt32(area);
		pk.pushInt32(load);
		pk.pushInt32(load_max);
		pk.pushString(access_url);
	}
};

struct PMCDeleteGrpReq : Packet {
	enum { uri=URI_MCENTER_DELETE_GRP_REQ };
	
	std::string	name;
	std::string ip;
	short	port;
	int		isp;
	int		area;	
	int		load_max;
	int		load;
	
	virtual void	unmarshall(Unpack& up) {
		name = up.popString();				
		ip = up.popString();	
		port = up.popInt16();
		isp = up.popInt32();
		area = up.popInt32();
		load = up.popInt32();
		load_max = up.popInt32();		
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(name);	
		pk.pushString(ip);
		pk.pushInt16(port);
		pk.pushInt32(isp);
		pk.pushInt32(area);
		pk.pushInt32(load);
		pk.pushInt32(load_max);
	}
};

struct PMCPing : Packet {
	enum { uri=URI_MCENTER_PING };
};

struct PMCAllocReq : Packet {
	enum { uri=URI_MCENTER_ALLOC_REQ };		
	
	int  	isp1;
	int 	area1;
	int  	isp2;
	int 	area2;
	
	virtual void	unmarshall(Unpack& up) {				
		isp1 = up.popInt32();
		area1 = up.popInt32();		
		isp2 = up.popInt32();
		area2 = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {				
		pk.pushInt32(isp1);
		pk.pushInt32(area1);		
		pk.pushInt32(isp2);
		pk.pushInt32(area2);
	}
};

struct PMCAllocRes : Packet {
	enum { uri=URI_MCENTER_ALLOC_RES };	
	
	short	res;	
	std::string	ip;	
	short	port;	
	
	int  	isp1;
	int 	area1;
	
	int  	isp2;
	int 	area2;
	
	virtual void	unmarshall(Unpack& up) {		
		res = up.popInt16();		
		ip = up.popString();
		port = up.popInt16();	

		isp1 = up.popInt32();
		area1 = up.popInt32();
		isp2 = up.popInt32();
		area2 = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt16(res);		
		pk.pushString(ip);
		pk.pushInt16(port);	
		
		pk.pushInt32(isp1);
		pk.pushInt32(area1);
		pk.pushInt32(isp2);
		pk.pushInt32(area2);
	}
};


struct PMCPreAllocReq : Packet {
	enum { uri=URI_MCENTER_PREALLOC_REQ };
	int  	isp1;
	int 	area1;
	int  	isp2;
	int 	area2;
	int		num;
	
	virtual void	unmarshall(Unpack& up) {		
		isp1 = up.popInt32();
		area1 = up.popInt32();		
		isp2 = up.popInt32();
		area2 = up.popInt32();
		num = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushInt32(isp1);
		pk.pushInt32(area1);		
		pk.pushInt32(isp2);
		pk.pushInt32(area2);
		pk.pushInt32(num);
	}
};

struct PMCPreAllocRes : Packet {
	enum { uri=URI_MCENTER_PREALLOC_RES };
	
	short	res;
	int  	isp1;
	int 	area1;
	int  	isp2;
	int 	area2;
	int		num;
	std::string	ip;
	short	port;
	
	virtual void	unmarshall(Unpack& up) {
		res = up.popInt16();
		
		isp1 = up.popInt32();
		area1 = up.popInt32();		
		isp2 = up.popInt32();
		area2 = up.popInt32();	
		num = up.popInt32();
		
		ip = up.popString();
		port = up.popInt16();		
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt16(res);
		
		pk.pushInt32(isp1);
		pk.pushInt32(area1);		
		pk.pushInt32(isp2);
		pk.pushInt32(area2);
		pk.pushInt32(num);		
		
		pk.pushString(ip);	
		pk.pushInt16(port);
	}
};

#endif
