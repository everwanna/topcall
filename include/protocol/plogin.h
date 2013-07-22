#ifndef _PLOGIN_H_
#define _PLOGIN_H_

#include "core/packet.h"
#include <vector>

namespace login {

enum {
	URI_LOGIN_REQ			= 1,
	URI_LOGIN_RES			= 2,
	URI_SEND_REQ			= 3,
	URI_SEND_RES			= 4,
	URI_SYNCUIDS_REQ		= 5,
	URI_SYNCUIDS_RES		= 6,
	URI_MULTILOGIN_NOTIFY	= 7,

	URI_REGPROXY_REQ		= 100,
	URI_REGPROXY_RES		= 101,
	URI_REGDISP_REQ			= 102,
	URI_REGDISP_RES			= 103,
	URI_REGROUTER_REQ		= 104,
	URI_REGROUTER_RES		= 105,
};

struct PLoginReq : Packet {	
	enum { uri=URI_LOGIN_REQ };

	int			uid;
	std::string	passport;
	std::string	password;

	virtual void	unmarshall(Unpack& up) {		
		uid = up.popInt32();
		passport = up.popString();
		password = up.popString();
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushInt32(uid);
		pk.pushString(passport);
		pk.pushString(password);		
	}
};

struct PLoginRes : Packet {	
	enum { uri=URI_LOGIN_RES };

	int			res;
	int			uid;
	std::string	cookie;

	virtual void	unmarshall(Unpack& up) {		
		res = up.popInt32();
		uid = up.popInt32();
		cookie = up.popString();
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushInt32(res);
		pk.pushInt32(uid);
		pk.pushString(cookie);
	}
};

struct PSendReq : Packet {	
	enum { uri=URI_SEND_REQ };

	int			uid;
	int			peer;
	int			seq;
	int			last_seq;
	std::string	data;

	virtual void	unmarshall(Unpack& up) {		
		uid = up.popInt32();
		peer = up.popInt32();
		seq = up.popInt32();
		last_seq = up.popInt32();
		data = up.popString();
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushInt32(uid);
		pk.pushInt32(peer);
		pk.pushInt32(seq);
		pk.pushInt32(last_seq);
		pk.pushString(data);
	}
};

struct PSendRes : Packet {	
	enum { uri=URI_SEND_RES };

	int			res;
	int			uid;
	int			peer;
	int			seq;

	virtual void	unmarshall(Unpack& up) {	
		res = up.popInt32();
		uid = up.popInt32();
		peer = up.popInt32();
		seq = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushInt32(res);
		pk.pushInt32(uid);
		pk.pushInt32(peer);
		pk.pushInt32(seq);
	}
};


struct PSyncUidsReq : Packet {	
	enum { uri=URI_SYNCUIDS_REQ };

	std::vector<int>	uids;

	virtual void	unmarshall(Unpack& up) {	
		int len = up.popInt16();
		for( int i=0; i<len; i++ ) {
			uids.push_back( up.popInt32() );
		}
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushInt16( uids.size() );
		for( size_t i=0; i<uids.size(); i++ ) {
			uids.push_back( uids[i] );
		}
	}
};

struct PMutiLoginNotify : Packet {
	enum { uri=URI_MULTILOGIN_NOTIFY };

	std::string		new_router;
	std::string		new_dispatcher;
	std::string		new_proxy;
	std::string		old_router;
	std::string		old_dispatcher;
	std::string		old_proxy;

	virtual void	unmarshall(Unpack& up) {	
		new_router = up.popString();
		new_dispatcher = up.popString();
		new_proxy = up.popString();
		old_router = up.popString();
		old_dispatcher = up.popString();
		old_proxy = up.popString();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushString( new_router );
		pk.pushString( new_dispatcher );
		pk.pushString( new_proxy );
		pk.pushString( old_router );
		pk.pushString( old_dispatcher );
		pk.pushString( old_proxy );
	}
};

struct PRegProxyReq : Packet {
	enum { uri=URI_REGPROXY_REQ };

	std::string		proxy;

	virtual void	unmarshall(Unpack& up) {	
		proxy = up.popString();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushString(proxy);
	}
};

struct PRegProxyRes : Packet {
	enum { uri=URI_REGPROXY_RES };

	std::string		proxy;
	std::string		dispatcher;
	std::string		router;

	virtual void	unmarshall(Unpack& up) {	
		proxy = up.popString();
		dispatcher = up.popString();
		router = up.popString();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushString(proxy);
		pk.pushString(dispatcher);
		pk.pushString(router);
	}
};

struct PRegDispReq : Packet {
	enum { uri=URI_REGDISP_REQ };

	std::string		dispatcher;

	virtual void	unmarshall(Unpack& up) {	
		dispatcher = up.popString();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushString(dispatcher);
	}
};

struct PRegDispRes : Packet {
	enum { uri=URI_REGDISP_RES };

	std::string		dispatcher;
	std::string		router;

	virtual void	unmarshall(Unpack& up) {	
		dispatcher = up.popString();
		router = up.popString();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushString(dispatcher);
		pk.pushString(router);
	}
};

struct PRegRouterReq : Packet {	
	enum { uri=URI_REGROUTER_REQ };

	std::string		name;

	virtual void	unmarshall(Unpack& up) {	
		name = up.popString();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushString(name);
	}
};


struct PRegRouterRes : Packet {	
	enum { uri=URI_REGROUTER_RES };

	int				res;
	std::string		name;

	virtual void	unmarshall(Unpack& up) {	
		res = up.popInt32();
		name = up.popString();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushInt32(res);
		pk.pushString(name);
	}
};

}

#endif