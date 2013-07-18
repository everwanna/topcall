#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class MCPMgr;
class MsgHandler {
public:
	MsgHandler(MCPMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, char* msg, int len);
	
public:
	void	onMCAllocReq(int linkid, Unpack* up);
	void	onMCPreAllocRes(int linkid, Unpack* up);

private:
	MCPMgr*	m_pMCPMgr;
	Pack	m_badGrpPack;
	Pack	m_packPreAllocRes;
	Pack	m_packAllocRes;
};

#endif
