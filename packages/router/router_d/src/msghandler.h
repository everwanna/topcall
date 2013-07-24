#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class RouteMgr;
class MsgHandler {
public:
	MsgHandler(RouteMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, const char* msg, int len);

public:
	void	onRegDispReq(int linkid, Unpack* up);
	void	onSendReq(int linkid, Unpack* up);
	void	onPushMsg(int linkid, Unpack* up);

private:
	RouteMgr*	m_pMgr;
	char*		m_pBuffer;
	Pack*		m_pPack;
};

#endif
