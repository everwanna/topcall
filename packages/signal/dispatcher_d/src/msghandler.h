#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class DispMgr;
class MsgHandler {
public:
	MsgHandler(DispMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, char* msg, int len);

public:
	void	onRegProxyReq(int linkid, Unpack* up);
	void	onRegDispRes(int linkid, Unpack* up);
	void	onSendReq(int linkid, Unpack* up);
	void	onSyncUidsReq(int linkid, Unpack* up);

private:
	DispMgr*	m_pDispMgr;
	char*		m_pBuffer;
};

#endif
