#ifndef _MSGCACHEMGR_H_
#define _MSGCACHEMGR_H_

#include "config.h"
#include <map>

class MsgCache;
class Msg;
class MsgCacheMgr {
public:
	MsgCacheMgr();
	~MsgCacheMgr();

public:
	void	addCache(int uid, int seq, Msg* msg);
	Msg*	getCache(int uid, int seq);
	void	remove(int uid);

private:
	//uid=>cache map.
	std::map<int, MsgCache*>	m_mapCaches;
};

#endif
