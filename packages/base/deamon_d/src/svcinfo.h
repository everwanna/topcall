#ifndef _SVCINFO_H_
#define _SVCINFO_H_

#include "config.h"

class SvcInfo {
public:
	SvcInfo(const std::string& name, const std::string& ip, short port, short svid, int isp, int area, int load, int laod_max, int stamp);

public:
	void	setIP(const std::string& ip) { m_strIp = ip; }
	void	setPort(short port) { m_nPort = port; }
	void	setSvid(short svid) { m_nSvid = svid; }
	void	setIsp(int isp) { m_nIsp = isp; }
	void	setArea(int area) { m_nArea = area; }
	void	setLoad(int load) { m_nLoad = load; }
	void	setLoadMax(int max) { m_nLoadMax = max; }
	void	setStamp(int stamp) { m_nStamp = stamp; }

public:
	std::string getName() { return m_strName; }
	short	getSvid() { return m_nSvid; }
	std::string getIP() { return m_strIp; }
	short	getPort() { return m_nPort; }
	int		getIsp() { return m_nIsp; }
	int		getArea() { return m_nArea; }
	int		getLoad() { return m_nLoad; }
	int		getLoadMax() { return m_nLoadMax; }
	int		getStamp() { return m_nStamp; }

private:	
	std::string	m_strName;
	std::string	m_strIp;
	short	m_nPort;
	short	m_nSvid;
	int		m_nIsp;
	int		m_nArea;
	int		m_nLoad;
	int		m_nLoadMax;
	int		m_nStamp;
};

#endif