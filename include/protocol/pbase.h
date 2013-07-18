#ifndef _PBASE_H
#define _PBASE_H

#include "core/packet.h"

struct PProxyInfo : Packet {		
	std::string	ip;
	short		port;
	int			isp;
	int			area;

	virtual void	unmarshall(Unpack& up) {		
		ip = up.popString();
		port = up.popInt16();
		isp = up.popInt32();
		area = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushString(ip);
		pk.pushInt16(port);
		pk.pushInt32(isp);
		pk.pushInt32(area);
	}
};

#endif