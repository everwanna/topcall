#ifndef _ISPINFO_H_
#define _ISPINFO_H_

#include "config.h"
#include <set>

class IspInfo {
public:
	IspInfo();

public:
	bool	isValidIsp(int isp);
	bool	isValidArea(int area);

private:
	std::set<int>	m_setIsps;
	std::set<int>	m_setAreas;
};

#endif
