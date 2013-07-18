#ifndef _SVCLIST_H_
#define _SVCLIST_H_

#include "config.h"
#include <list>
#include <map>

class SvcInfo;
class SvcList {
public:
	SvcList(short svc);
	~SvcList();

public:
	void	add(const std::string& name, const std::string& ip, short port, short svid, int isp, int area, int load, int laod_max, int stamp);	
	void	getSvcInfos(PDQueryRes& res);
	void	removeExpire(int time);

public:
	std::map<std::string, SvcInfo*>	m_mapSvcs;	
	short	m_nSvc;
};

#endif