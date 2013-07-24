#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "config.h"

class PullMgr;
class MsgHandler {
public:
	MsgHandler(PullMgr* mgr);
	~MsgHandler();

public:
	void	handle(int linkid, char* msg, int len);

public:
	void	onConsumerRegisterReq(int linkid, Unpack* up);
	void	onPushMsg(int linkid, Unpack* up);

private:
	PullMgr*	m_pPushMgr;
};

#endif