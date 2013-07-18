#ifndef _PMDEAMON_H_
#define _PMDEAMON_H_

#include "core/packet.h"
#include <vector>

#define URI_MDEAMON_REPORT			1
#define URI_MDEAMON_QUERY_REQ		100
#define URI_MDEAMON_QUERY_RES		100

struct PDSvcInfo : Packet {	
	std::string ip;
	short		port;
	short		svid;
	int			isp;
	int			area;	
	int			load;
	int			load_max;
	
	virtual void	unmarshall(Unpack& up) {		
		ip = up.popString();	
		port = up.popInt16();
		svid = up.popInt16();
		isp = up.popInt32();
		area = up.popInt32();		
		load = up.popInt32();
		load_max = up.popInt32();		
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushString(ip);
		pk.pushInt16(port);
		pk.pushInt16(svid);
		pk.pushInt32(isp);
		pk.pushInt32(area);	
		pk.pushInt32(load);
		pk.pushInt32(load_max);
	}
};

struct PDReport : Packet {
	enum { uri=URI_MDEAMON_REPORT };
	
	std::string	name;	//usually idc name.
	std::string	ip;
	short	port;
	short	svid;
	int		isp;
	int		area;	
	int		load_max;
	int		load;
	
	virtual void	unmarshall(Unpack& up) {
		name = up.popString();				
		ip = up.popString();	
		port = up.popInt16();
		svid = up.popInt16();
		isp = up.popInt32();
		area = up.popInt32();
		load = up.popInt32();
		load_max = up.popInt32();		
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(name);	
		pk.pushString(ip);
		pk.pushInt16(port);
		pk.pushInt16(svid);
		pk.pushInt32(isp);
		pk.pushInt32(area);
		pk.pushInt32(load);
		pk.pushInt32(load_max);
	}
};

struct PDQueryReq : Packet {
	enum { uri=URI_MDEAMON_QUERY_REQ };
	short	svid;
	short	seq;
		
	virtual void	unmarshall(Unpack& up) {
		svid = up.popInt16();
		seq = up.popInt16();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt16(svid);
		pk.pushInt16(seq);
	}
};

struct PDQueryRes : Packet {
	enum { uri=URI_MDEAMON_QUERY_RES };
	short	res;
	short	svid;	
	std::vector<PDSvcInfo>	infos;
		
	virtual void	unmarshall(Unpack& up) {
		res = up.popInt16();
		svid = up.popInt16();

		short len  = up.popInt16();
		for( int i=0; i<len; i++ ) {
			PDSvcInfo info;
			info.unmarshall(up);
		}
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt16(res);
		pk.pushInt16(svid);		

		for( size_t i=0; i<infos.size(); i++ ) {
			infos[i].marshall(pk);
		}
	}
};


#endif
