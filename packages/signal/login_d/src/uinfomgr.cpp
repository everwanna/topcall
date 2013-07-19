#include "uinfomgr.h"
#include "uinfo.h"

UInfoMgr::UInfoMgr() {
}

UInfoMgr::~UInfoMgr() {
}

void	UInfoMgr::add(UInfo* uinfo) {
	//we should not need a verification here.
	m_mapUInfos[uinfo->uid] = uinfo;
}

UInfo*	UInfoMgr::get(int uid) {
	std::map<int, UInfo*>::iterator it = m_mapUInfos.find(uid);
	if( it == m_mapUInfos.end() ) 
		return NULL;

	return it->second;
}

