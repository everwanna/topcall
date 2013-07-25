#include "statmgr.h"

StatMgr::StatMgr() 
	: m_nUserCount(0)
{
}

void	StatMgr::onUserLogin() {
	m_nUserCount++;
}

void	StatMgr::onUserLogout() {
	m_nUserCount--;
}
