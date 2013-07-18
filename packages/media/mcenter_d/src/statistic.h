#ifndef _STATISTIC_H_
#define _STATISTIC_H_

#include "config.h"
#include <map>

class MCMgr;
class Statistic {
public:
	Statistic(MCMgr* mgr);
	
public:
	void	addIspReq(const std::string& isp);
	int		getFps();
	void	dump();

private:
	MCMgr*	m_pMCMgr;
	std::map<std::string, int>	m_mapIspReqs;
	int		m_nStartTime;
};

#endif
