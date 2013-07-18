#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class LTMgr;
class MsgHandler {
public:
	MsgHandler(LTMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, char* msg, int len);

public:
	void	onLTReport(int linkid, Unpack* up);
	void	onLTQueryReq(int linkid, Unpack* up);
	void	onLTQueryJsonReq(int linkid, Unpack* up);
	void	onLTRegSlaveReq(int linkid, Unpack* up);
	void	onLTPing(int linkid, Unpack* up);

private:
	LTMgr*		m_pLTMgr;
	PLTQueryRes	m_queryRes;
	char*		m_pBuffer;
};

#endif
