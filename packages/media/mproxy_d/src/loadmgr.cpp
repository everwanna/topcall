#include "loadmgr.h"
#include "mpmgr.h"

LoadMgr::LoadMgr(MPMgr* mgr, int max) 
	: m_pMPMgr(mgr)
	, m_nLoadMax(max) {
}

LoadMgr::~LoadMgr() {
}

int		LoadMgr::getLoad() {
	//return m_pMPMgr->getProdMgr()->getCount() + m_pMPMgr->getConsMgr()->getCount();
	return 0;
}

int		LoadMgr::getMaxLoad() {
	return m_nLoadMax;
}

bool	LoadMgr::hasLoad() {
	return (m_nLoadMax>getLoad());
}
