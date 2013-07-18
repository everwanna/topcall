#ifndef _PLOGIN_H_
#define _PLOGIN_H_

#include "core/packet.h"
#include <vector>

namespace login {

enum {
	URI_LOGIN_REQ		= 1,
	URI_LOGIN_RES		= 2,
	URI_SEND_REQ		= 3,
	URI_SEND_RES		= 4,
	URI_SYNCUIDS_REQ	= 5,
	URI_SYNCUIDS_RES	= 6,
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
	std::string	data;

	virtual void	unmarshall(Unpack& up) {		
		uid = up.popInt32();
		peer = up.popInt32();
		seq = up.popInt32();
		data = up.popString();
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushInt32(uid);
		pk.pushInt32(peer);
		pk.pushInt32(seq);
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

}

#endif