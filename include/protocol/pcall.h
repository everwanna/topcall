#ifndef PCALL_H
#define PCALL_H

#include "core/packet.h"

#define URI_CALL_INVITE_REQ		1
#define URI_CALL_INVITE_RES		1

struct PCallInviteReq : Packet {
	enum { uri=URI_CALL_INVITE_REQ };
	int	inviter;
	int	invitee;
	int	stamp_inviter;	
	int	stamp_invitee;
	int	stamp_server;
	String	nick;
	
	virtual void	unmarshall(Unpack& up) {
		inviter = up.popInt32();
		invitee = up.popInt32();
		stamp_inviter = up.popInt32();
		stamp_invitee = up.popInt32();
		stamp_server = up.popInt32();
		nick = up.popString();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(inviter);
		pk.pushInt32(invitee);
		pk.pushInt32(stamp_inviter);
		pk.pushInt32(stamp_invitee);
		pk.pushInt32(stamp_server);
		pk.pushString(nick);		
	}
};

struct PCallInviteRes : Packet {
	enum { uri=URI_CALL_INVITE_RES };
	enum {
		INVITE_ACCEPT = 1;
		INVITE_DENY = 2;
		INVITE_IGNORE = 2;
		INVITE_LATER = 2;
	};
	int	inviter;
	int	invitee;
	int	stamp_inviter;	
	int	stamp_invitee;
	int	stamp_server;	
	int	response;
	
	virtual void	unmarshall(Unpack& up) {
		inviter = up.popInt32();
		invitee = up.popInt32();
		stamp_inviter = up.popInt32();
		stamp_invitee = up.popInt32();
		stamp_server = up.popInt32();		
		response = up.popInt32();
	}

	virtual void	marshall(Pack& pk) {
		pk.pushInt32(inviter);
		pk.pushInt32(invitee);
		pk.pushInt32(stamp_inviter);
		pk.pushInt32(stamp_invitee);
		pk.pushInt32(stamp_server);
		pk.pushInt32(response);	
	}
};


#endif