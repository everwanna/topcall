#ifndef PMPROXY_H
#define PMPROXY_H

#include "core/packet.h"

#define URI_MPROXY_CREATE_REQ			1
#define URI_MPROXY_CREATE_RES			2

#define URI_MPROXY_JOIN_REQ				5
#define URI_MPROXY_JOIN_RES				6
#define URI_MPROXY_LEAVE_REQ			7
#define URI_MPROXY_LEAVE_RES			8
#define URI_MPROXY_STREAM_EVENT			9
#define URI_MPROXY_PING					10

#define URI_MPROXY_STREAM_DATA			100
#define URI_MPROXY_STREAM_DATA2			101
#define URI_MPROXY_RESEND_REQ			102

struct PMPCreateReq : Packet {
	enum { uri=URI_MPROXY_CREATE_REQ };
	
	int			uid;
	std::string	nick;
	std::string	stream;
	virtual void	unmarshall(Unpack& up) {
		uid = up.popInt32();
		nick = up.popString();
		stream = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(uid);
		pk.pushString(nick);
		pk.pushString(stream);
	}
};


struct PMPCreateRes : Packet {
	enum { uri=URI_MPROXY_CREATE_RES };
	
	int			res;
	int			sid;
	int			uid;
	std::string	nick;
	std::string	stream;
	virtual void	unmarshall(Unpack& up) {
		res = up.popInt32();
		sid = up.popInt32();
		uid = up.popInt32();
		nick = up.popString();
		stream = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(res);
		pk.pushInt32(sid);
		pk.pushInt32(uid);
		pk.pushString(nick);
		pk.pushString(stream);
	}
};

struct PMPJoinReq : Packet {
	enum { uri=URI_MPROXY_JOIN_REQ };
	
	int			sid;
	int			uid;
	std::string	nick;
	std::string	stream;
	virtual void	unmarshall(Unpack& up) {
		sid = up.popInt32();
		uid = up.popInt32();
		nick = up.popString();
		stream = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(sid);
		pk.pushInt32(uid);
		pk.pushString(nick);
		pk.pushString(stream);
	}
};

struct PMPJoinRes : Packet {
	enum { uri=URI_MPROXY_JOIN_RES };
	
	int			res;
	int			sid;
	int			uid;	
	
	virtual void	unmarshall(Unpack& up) {
		res = up.popInt32();
		sid = up.popInt32();
		uid = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(res);
		pk.pushInt32(sid);
		pk.pushInt32(uid);		
	}
};

struct PMPLeaveReq : Packet {
	enum { uri=URI_MPROXY_LEAVE_REQ };
	
	int			uid;
	std::string	nick;
	std::string	stream;
	virtual void	unmarshall(Unpack& up) {
		uid = up.popInt32();
		nick = up.popString();
		stream = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(uid);
		pk.pushString(nick);
		pk.pushString(stream);
	}
};

struct PMPLeaveRes : Packet {
	enum { uri=URI_MPROXY_LEAVE_RES };
	
	int			uid;
	std::string	nick;
	std::string	stream;
	int			res;
	virtual void	unmarshall(Unpack& up) {
		uid = up.popInt32();
		nick = up.popString();
		stream = up.popString();
		res = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(uid);
		pk.pushString(nick);
		pk.pushString(stream);
		pk.pushInt32(res);
	}
};

struct PMPStreamEvent : Packet {
	enum { uri=URI_MPROXY_STREAM_EVENT };	
	
	enum {
		EVENT_UNPUBLISH,
		EVENT_UNSUBSCRIBE,
	};

	int		event;
	int		from;
	std::string	stream;
	virtual void	unmarshall(Unpack& up) {
		event = up.popInt32();
		from = up.popInt32();
		stream = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(event);
		pk.pushInt32(from);
		pk.pushString(stream);
	}
};

struct PMPPing : Packet {
	enum { uri=URI_MPROXY_PING };	
	int		sid;
	int		from;
	std::string	stream;
	virtual void	unmarshall(Unpack& up) {
		sid = up.popInt32();
		from = up.popInt32();		
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushInt32(sid);
		pk.pushInt32(from);				
	}
};

struct PMPStreamData : Packet {
	enum { uri=URI_MPROXY_STREAM_DATA };	
	
	int			sid;
	int			from;
	int			seq;
	std::string payload;

	virtual void	unmarshall(Unpack& up) {	
		sid = up.popInt32();
		from = up.popInt32();
		seq = up.popInt32();
		//this is a stupid hack, trying to hook up a memory pool.
		//later I should implement the memory pool here.
		//payload = up.popString();
	}

	virtual void	marshall(Pack& pk) {	
		pk.pushInt32(sid);
		pk.pushInt32(from);
		pk.pushInt32(seq);
		pk.pushString(payload);
	}
};

struct PMPStreamData2 : Packet {
	enum { uri=URI_MPROXY_STREAM_DATA2 };	
	
	int			seq;
	int			stamp;
	std::string payload;

	virtual void	unmarshall(Unpack& up) {	
		seq = up.popInt32();
		stamp = up.popInt32();
		//this is a stupid hack, trying to hook up a memory pool.
		//later I should implement the memory pool here.
		//payload = up.popString();
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushInt32(seq);
		pk.pushInt32(stamp);
		pk.pushString(payload);
	}
};

struct PMPResendReq : Packet {
	enum { uri=URI_MPROXY_RESEND_REQ };	

	int			from;
	int			seq;
	virtual void	unmarshall(Unpack& up) {	
		from = up.popInt32();
		seq = up.popInt32();		
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushInt32(from);
		pk.pushInt32(seq);		
	}
};


#endif
