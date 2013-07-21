#ifndef _UINFOMGR_H_
#define _UINFOMGR_H

#include "config.h"
#include <map>

struct UInfo;
class UInfoMgr {
public:
	UInfoMgr();
	~UInfoMgr();

public:
	void	add(UInfo* uinfo);
	UInfo*	get(int uid);

private:
	//uid=>UInfo map:
	std::map<int, UInfo*>	m_mapUInfos;
};

#endif

