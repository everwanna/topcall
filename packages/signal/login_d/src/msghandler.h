#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class LoginMgr;
class MsgHandler {
public:
	MsgHandler(LoginMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, char* msg, int len);

public:
	void	onRegProxyRes(int linkid, Unpack* up);
	void	onLoginReq(int linkid, Unpack* up);
	void	onSendReq(int linkid, Unpack* up);
	void	onPing(int linkid, Unpack* up);
	void	onQueryJsonReq(int linkid, Unpack* up);
	void	onCreateRoomReq(int linkid, Unpack* up);

private:
	LoginMgr*	m_pLoginMgr;
	char*		m_pBuffer;
	Pack*		m_pPack;
};

#endif
