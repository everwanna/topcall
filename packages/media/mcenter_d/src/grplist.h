#ifndef _GRPLIST_H_
#define _GRPLIST_H_

#include "config.h"
#include <list>

class GrpInfo;
class GrpList {
public:
	GrpList();
	~GrpList();

public:
	void		addGroup(GrpInfo* group);
	void		remvoeGroup(const std::string& group);
	GrpInfo*	getFreeGroup(int num);

private:
	std::list<GrpInfo*>	m_arrGroups;
};

#endif
