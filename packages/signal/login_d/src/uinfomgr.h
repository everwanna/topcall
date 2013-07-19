#ifndef _UINFOMGR_H_
#define _UINFOMGR_H

#include "config.h"
#include <map>

class UInfo;
class UInfoMgr {
public:
	UInfoMgr();
	~UInfoMgr();

public:
	void	add(int uid, int linkid, const std::string& passport, const std::string& password);
	UInfo*	get(int uid);

private:
	std::map<int, UInfo*>	m_mapUInfos;
};

#endif

