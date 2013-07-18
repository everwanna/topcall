#include "statistic.h"
#include "mcmgr.h"
#include "netloop.h"

Statistic::Statistic(MCMgr* mgr)
	: m_pMCMgr(mgr)
{
	m_nStartTime = m_pMCMgr->getLooper()->getSystemTime();
}

void	Statistic::addIspReq(const std::string& isp) {
	std::map<std::string, int>::iterator it = m_mapIspReqs.find(isp);
	if( it == m_mapIspReqs.end() ) {
		m_mapIspReqs[isp] = 1;
	} else {
		it->second++;
	}
}

int		Statistic::getFps() {
	int	time = m_pMCMgr->getLooper()->getSystemTime();
	int interval = time - m_nStartTime;
	int num = 0;

	if( interval == 0 )
		return 0;
	
	for( std::map<std::string, int>::iterator it= m_mapIspReqs.begin(); it != m_mapIspReqs.end(); it++ ) {
		num += it->second;
	}

	return num*1000/interval;
}

void	Statistic::dump() {
	//LOG(TAG_MCENTER, "Statistic::dump, fps=%d.", getFps());
	for( std::map<std::string, int>::iterator it = m_mapIspReqs.begin(); it != m_mapIspReqs.end(); it++ ) {
		LOG(TAG_MCENTER, "Statistic.dump, isp/num=%s, %d", it->first.c_str(), it->second);
	}
}

