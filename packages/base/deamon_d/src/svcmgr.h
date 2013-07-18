#ifndef _SVCMGR_H_
#define _SVCMGR_H_

#include "config.h"
#include <map>

class SvcList;
class SvcInfo;
class SvcMgr {
public:
	SvcMgr();
	~SvcMgr();

public:
	void		add(const std::string& name, const std::string& ip, short port, short svid, int isp, int area, int load, int laod_max, int stamp);
	SvcList*	query(short svid);
	void		removeExpire(int time);

private:
	std::map<short, SvcList*>	m_mapSvcs;
};

#endif