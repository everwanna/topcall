#ifndef _PVOICECALL_H_
#define _PVOICECALL_H_

#include "core/packet.h"

#define URI_VCALL_INVITE_REQ		100
#define URI_VCALL_INVITE_RES		101

#define URI_VCALL_JOIN_REQ		102
#define URI_VCALL_JOIN_RES		103

struct PVCallInviteReq : Packet {
	enum { uri=URI_VCALL_INVITE_REQ };
	int		inviter;
	int		invitee;
	std::string	nick;	
	
	virtual void	unmarshall(Unpack& up) {
		inviter = up.popInt32();
		invitee = up.popInt32();
		nick = up.popString();		
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(inviter);
		pk.pushInt32(invitee);
		pk.pushString(nick);		
	}
};

struct PVCallInviteRes : Packet {
	enum { uri=URI_MPROXY_PUBLIC_RES };
	
	int		inviter;
	int		invitee;
	std::string	nick;	
	int		res;
	virtual void	unmarshall(Unpack& up) {
		inviter = up.popInt32();
		invitee = up.popInt32();
		nick = up.popString();	
		res = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(inviter);
		pk.pushInt32(invitee);
		pk.pushString(nick);
		pk.pushInt32(res);
	}
};

struct PVCallJoinReq : Packet {
	enum { uri=URI_VCALL_JOIN_REQ };
	int		inviter;
	int		invitee;
	std::string	ip;	
	short		port;
	
	virtual void	unmarshall(Unpack& up) {
		inviter = up.popInt32();
		invitee = up.popInt32();
		ip = up.popString();	
		invitee = up.popInt16();	
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(inviter);
		pk.pushInt32(invitee);
		pk.pushString(ip);		
		pk.pushInt16(port);
	}
};

struct PVCallJoinRes : Packet {
	enum { uri=URI_VCALL_JOIN_RES };
	
	int		inviter;
	int		invitee;		
	int		res;
	virtual void	unmarshall(Unpack& up) {
		inviter = up.popInt32();
		invitee = up.popInt32();
		res = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(inviter);
		pk.pushInt32(invitee);
		pk.pushInt32(res);
	}
};


#endif