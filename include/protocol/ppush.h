#ifndef PPUSH_H
#define PPUSH_H

#include "core/packet.h"

#define URI_PRODUCER_REGISTER_REQ	1
#define URI_PRODUCER_REGISTER_RES	2
#define URI_CONSUMER_REGISTER_REQ	3
#define URI_CONSUMER_REGISTER_RES	4
#define URI_PUSH_MSG				5

struct PProducerRegisterReq : Packet {
	enum { uri=URI_PRODUCER_REGISTER_REQ };
	std::string		name;
	virtual void	unmarshall(Unpack& up) {
		name = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(name);
	}
};

struct PProducerRegisterRes : Packet {
	enum { uri=URI_PRODUCER_REGISTER_RES };
	
	std::string		name;
	virtual void	unmarshall(Unpack& up) {
		name = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(name);
	}
};


struct PConsumerRegisterReq : Packet {
	enum { uri=URI_CONSUMER_REGISTER_REQ };
	
	std::string	name;
	std::string	topic;
	virtual void	unmarshall(Unpack& up) {
		name = up.popString();
		topic = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushString(name);
		pk.pushString(topic);
	}
};

struct PConsumerRegisterRes : Packet {
	enum { uri=URI_CONSUMER_REGISTER_RES };
	
	int			res;
	std::string	name;
	virtual void	unmarshall(Unpack& up) {
		res = up.popInt32();
		name = up.popString();		
		
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(res);
		pk.pushString(name);
	}
};

struct PPushMsg : Packet {
	enum { uri=URI_PUSH_MSG };
	
	std::string	topic;
	std::string payload;

	virtual void	unmarshall(Unpack& up) {	
		topic = up.popString();
		payload = up.popString();
	}

	virtual void	marshall(Pack& pk) {		
		pk.pushString(topic);
		pk.pushString(payload);
	}
};


#endif