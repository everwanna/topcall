#ifndef _PLOADTABLE_H_
#define _PLOADTABLE_H_

#include "core/packet.h"

#define URI_LOAD_REPORT				1
#define URI_LOAD_QUERY_REQ			2
#define URI_LOAD_QUERY_RES			3

#define URI_LOAD_QUERYJSON_REQ		4
#define URI_LOAD_QUERYJSON_RES		5

#define URI_LOAD_REGSLAVE_REQ		6
#define URI_LOAD_REGSLAVE_RES		7

#define URI_LOAD_PING				8

struct PLTReport : Packet {	
	enum { uri=URI_LOAD_REPORT };

	std::string	key;
	std::string	ip;
	short		port;
	int			load;
	int			load_max;
	std::string	ext;

	virtual void	unmarshall(Unpack& up) {
		key = up.popString();		
		ip = up.popString();
		port = up.popInt16();
		load = up.popInt32();
		load_max = up.popInt32();
		ext = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(key);
		pk.pushString(ip);
		pk.pushInt16(port);
		pk.pushInt32(load);
		pk.pushInt32(load_max);
		pk.pushString(ext);
	}
};

struct PLTQueryReq : Packet {	
	enum { uri=URI_LOAD_QUERY_REQ };
	virtual void	unmarshall(Unpack& up) {
		
	}

	virtual void	marshall(Pack& pk) {		
	}
};


struct PLTQueryRes : Packet {	
	int			res;
	std::string	key;
	std::string	ip;
	short		port;
	int			load;
	int			load_max;

	virtual void	unmarshall(Unpack& up) {
		res = up.popInt32();
		key = up.popString();		
		ip = up.popString();
		port = up.popInt16();
		load = up.popInt32();
		load_max = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(res);
		pk.pushString(key);
		pk.pushString(ip);
		pk.pushInt16(port);
		pk.pushInt32(load);
		pk.pushInt32(load_max);
	}
};

struct PLTQueryJsonReq : Packet {	
	enum { uri=URI_LOAD_QUERYJSON_REQ };

	virtual void	unmarshall(Unpack& up) {
	}

	virtual void	marshall(Pack& pk) {
	}
};

struct PLTQueryJsonRes : Packet {	
	enum { uri=URI_LOAD_QUERYJSON_RES };

	std::string		json;
	virtual void	unmarshall(Unpack& up) {
		json = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(json);
	}
};

struct PLTRegSlaveReq : Packet {	
	enum { uri=URI_LOAD_REGSLAVE_REQ };

	std::string		name;
	virtual void	unmarshall(Unpack& up) {
		name = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(name);
	}
};

struct PLTRegSlaveRes : Packet {	
	enum { uri=URI_LOAD_REGSLAVE_RES };

	virtual void	unmarshall(Unpack& up) {
	}

	virtual void	marshall(Pack& pk) {
	}
};

struct PLTPing : Packet {
	enum { uri=URI_LOAD_PING };
};

#endif

