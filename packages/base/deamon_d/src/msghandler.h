#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class MDMgr;
class MsgHandler {
public:
	MsgHandler(MDMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, char* msg, int len);

	void	onMDReport(int linkid, Unpack* up);
	void	onMDQueryReq(int linkid, Unpack* up);
	

private:
	MDMgr*	m_pMDMgr;
};

#endif