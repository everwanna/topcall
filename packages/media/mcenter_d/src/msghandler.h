#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class MCMgr;
class MsgHandler {
public:
	MsgHandler(MCMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, char* msg, int len);

public:
	void	onMCPing(int linkid, Unpack* up);
	void	onMCReportReq(int linkid, Unpack* up);
	void	onMCPreAllocReq(int linkid, Unpack* up);

private:
	MCMgr*	m_pMCMgr;
	Pack	m_badGrpPack;
};

#endif
