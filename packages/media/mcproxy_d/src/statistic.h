#ifndef _STATISTIC_H_
#define _STATISTIC_H_

#include "config.h"
#include <map>
#include <string>

class MCPMgr;
class Statistic {
public:
	Statistic(MCPMgr* mgr);
	
public:
	void	addIspReq(const std::string& isp);
	int		getFps();
	void	dump();

private:
	MCPMgr*	m_pMCPMgr;
	int		m_nStartTime;
	std::map<std::string, int>	m_mapIspReqs;	
};

#endif
