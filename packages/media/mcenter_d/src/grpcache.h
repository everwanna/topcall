#ifndef _GRPCACHE_H_
#define _GRPCACHE_H_

#include "config.h"
#include <map>
#include <string>

class GrpInfo;
class GrpCache {
public:
	GrpCache();
	~GrpCache();

public:
	GrpInfo*	get(int isp1, int area1, int isp2, int area2);
	void		put(int isp1, int area1, int isp2, int area2, GrpInfo* group);

private:
	std::map<std::string, GrpInfo*>	m_mapCache;
};

#endif
