#ifndef _GRPMGR_H_
#define _GRPMGR_H_

#include "config.h"
#include <map>
#include <vector>
#include <string>
#include <set>

class MCMgr;
class GrpInfo;
class GrpIspMap;
class GrpCache;
class GrpMgr {
public:
	GrpMgr(MCMgr* mgr);
	~GrpMgr();

public:
	void	add(int linkid, const std::string& name, const std::string& ip, short port, int isp, int area, int load, int load_max, const std::string& access_url);	
	void	remove(const std::string& name);
	GrpInfo* fetchGroup(int isp1, int area1, int isp2, int area2, int num);
	void	removeExpire(int time);
	void	dump();

private:
	GrpInfo*	findDiffIsp(int isp1, int area1, int isp2, int area2, int num);
	GrpInfo*	findFallbackIsp(int isp1, int area1, int isp2, int area2, int num);

private:
	MCMgr*		m_pMCMgr;
	//name=>GrpInfo map.
	std::set<int>	m_setIsps;
	std::map<std::string, GrpInfo*>	m_mapGroups;
	GrpIspMap*	m_pIspMap;
	GrpCache*	m_pCache;
};

#endif
