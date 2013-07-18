#ifndef _SLAVEMGR_H_
#define _SLAVEMGR_H_

#include "config.h"
#include <map>

struct Slave;
class LTMgr;
class SlaveMgr {
public:
	SlaveMgr(LTMgr* mgr);
	~SlaveMgr();

public:
	void	add(int linkid, const std::string& name, int time);
	void	remove(int linkid);
	void	send(const char* data, int len);
	void	update(int linkid, int time);
	void	removeExpired(int time);

private:
	LTMgr*	m_pLTMgr;
	std::map<int, Slave*>	m_mapSlaves;
};

#endif
