#ifndef _GRPISPMAP_H_
#define _GRPISPMAP_H_

#include "config.h"
#include <map>

class GrpInfo;
class GrpList;
class GrpIspMap {
public:
	GrpIspMap();
	~GrpIspMap();

public:
	void		add(int isp, int area, GrpInfo* info);
	void		remove(int isp, int area, const std::string& group);
	GrpInfo*	findGroup(int isp, int area, int num);

private:
	std::map<std::string, GrpList*>	m_mapIspGroups;	
};

#endif
