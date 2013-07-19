#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class RouteMgr;
class MsgHandler {
public:
	MsgHandler(RouteMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, char* msg, int len);

public:
	void	onRegRouterReq(int linkid, Unpack* up);
	void	onSendReq(int linkid, Unpack* up);

private:
	RouteMgr*	m_pLoginMgr;
	char*		m_pBuffer;
};

#endif
