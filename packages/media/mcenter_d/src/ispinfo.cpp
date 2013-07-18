#include "ispinfo.h"

IspInfo::IspInfo() {
	//isps:
	m_setIsps.insert(ISP_CTL);
	m_setIsps.insert(ISP_CNC);
	m_setIsps.insert(ISP_CM);
	m_setIsps.insert(ISP_EDU);
	m_setIsps.insert(ISP_HK);

	m_setIsps.insert(ISP_CTL_CNC);
	m_setIsps.insert(ISP_CTL_CM);
	m_setIsps.insert(ISP_CNC_CM);
	m_setIsps.insert(ISP_CTL_CNC_CM);

	m_setIsps.insert(ISP_ALL);

	//areas:
	m_setAreas.insert(AREA_BJ);
	m_setAreas.insert(AREA_SH);
	m_setAreas.insert(AREA_SZ);
	m_setAreas.insert(AREA_CQ);
	m_setAreas.insert(AREA_GZ);
	m_setAreas.insert(AREA_NE);
	m_setAreas.insert(AREA_N);
	m_setAreas.insert(AREA_NW);
	m_setAreas.insert(AREA_ME);
	m_setAreas.insert(AREA_M);
	m_setAreas.insert(AREA_MW);
	m_setAreas.insert(AREA_SE);
	m_setAreas.insert(AREA_S);
	m_setAreas.insert(AREA_SW);
}

bool	IspInfo::isValidIsp(int isp) {
	return m_setIsps.find(isp)!=m_setIsps.end();
}

bool	IspInfo::isValidArea(int area) {
	return m_setAreas.find(area)!=m_setAreas.end();
}

