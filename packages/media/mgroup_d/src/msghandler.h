#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class MGMgr;
class MsgHandler {
public:
	MsgHandler(MGMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, char* msg, int len);

public:
	void	onReportReq(int linkid, Unpack* up);
	void	onAllocProxyReq(int linkid, Unpack* up);

private:
	MGMgr*	m_pMGMgr;
};

#endif
