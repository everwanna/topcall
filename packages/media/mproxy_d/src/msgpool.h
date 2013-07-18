#ifndef _MSGPOOL_H
#define _MSGPOOL_H

#include "config.h"
#include <deque>

class Msg;
class MsgPool {
public:
	MsgPool();
	~MsgPool();

public:
	Msg*	fetch();
	void	free(Msg* msg);

private:
	std::deque<Msg*>	m_arrMsgs;
};

#endif
