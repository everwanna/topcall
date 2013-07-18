#include "grpinfo.h"

GrpInfo::GrpInfo(const std::string& name, const std::string& ip, short port) 
	: m_strName(name)
	, m_strIp(ip)
	, m_nPort(port)
{
	m_nIsp = 0;
	m_nArea = 0;
	m_nLoad = 0;
	m_nLoadMax = 0;
	m_nLoadAllocted = 0;
	m_nLastUpdate = 0;
}

GrpInfo::~GrpInfo() {
}

void	GrpInfo::setIsp(int isp) {
	m_nIsp = isp;
}

void	GrpInfo::setArea(int area) {
	m_nArea = area;
}

void	GrpInfo::setLoad(int load) {
	m_nLoad = load;
}

void	GrpInfo::setLoadMax(int max) {
	m_nLoadMax = max;
}

void	GrpInfo::addAllocated(int num) {
	m_nLoadAllocted += num;
}
