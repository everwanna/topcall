#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class PushMgr;
class MsgHandler {
public:
	MsgHandler(PushMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, char* msg, int len);

public:
	void	onConsumerRegisterReq(int linkid, Unpack* up);
	void	onPushMsg(int linkid, Unpack* up);

private:
	PushMgr*	m_pPushMgr;
};

#endif