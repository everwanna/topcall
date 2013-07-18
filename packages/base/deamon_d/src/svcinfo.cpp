#include "svcinfo.h"

SvcInfo::SvcInfo(const std::string& name, 
				 const std::string& ip, short port, 
				 short svid, int isp, int area, 
				 int load, int load_max, 
				 int stamp) 
	: m_strName(name)
	, m_strIp(ip)
	, m_nPort(port)
	, m_nSvid(svid)
	, m_nIsp(isp)
	, m_nArea(area)
	, m_nLoad(load)
	, m_nLoadMax(load_max)
	, m_nStamp(stamp)
{	
}