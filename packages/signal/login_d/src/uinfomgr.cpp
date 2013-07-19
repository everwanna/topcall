#include "uinfomgr.h"

UInfoMgr::UInfoMgr() {
}

UInfoMgr::~UInfoMgr() {
}

void	UInfoMgr::add(int linkid, int uid, const std::string& passport, const std::string& password) {

}

UInfo*	UInfoMgr::get(int uid) {
	std::map<int, UInfo*>::iterator it = m_mapUInfos.find(uid);
	if( it == m_mapUInfos.end() ) 
		return NULL;

	return it->second;
}

