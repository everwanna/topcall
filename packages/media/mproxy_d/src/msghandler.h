#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class MPMgr;
class MsgHandler {
public:
	MsgHandler(MPMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, const sockaddr_in* peer_addr, char* msg, int len);

public:	
	void	onCreateReq(int linkid, const sockaddr_in* peer_addr, Unpack* up);
	void	onMPJoinReq(int linkid, const sockaddr_in* peer_addr, Unpack* up);
	void	onMPLeaveReq(int linkid, const sockaddr_in* peer_addr, Unpack* up);
	void	onMPPing(int linkid, const sockaddr_in* peer_addr, Unpack* up);
	void	onMPStreamData(int linkid, const sockaddr_in* peer_addr, Unpack* up);
	void	onMPResendReq(int linkid, const sockaddr_in* peer_addr, Unpack* up);

private:
	MPMgr*	m_pMPMgr;
};

#endif
