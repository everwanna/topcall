#ifndef _GRPINFO_H_
#define _GRPINFO_H_

#include "config.h"

class GrpInfo {
public:
	GrpInfo(const std::string& name, const std::string& ip, short port);
	~GrpInfo();

public:
	void	setIsp(int isp);
	void	setArea(int area);
	void	setLoad(int load);
	void	setLoadMax(int max);
	void	setAccessUrl(const std::string& url) { m_strAccessUrl = url; }
	void	setLastUpdate(int time) { m_nLastUpdate = time; }
	void	addAllocated(int num);
	void	clearAllocated() { m_nLoadAllocted = 0; }

public:
	bool	isSame(const GrpInfo& other) ;
	std::string getName() { return m_strName; }
	std::string getIp() {return m_strIp; }
	short	getPort() { return m_nPort; }
	int		getIsp() { return m_nIsp; }
	int		getArea() { return m_nArea; }
	int		getLoad() { return m_nLoad; }
	int		getLoadMax() { return m_nLoadMax; }
	std::string getAccessUrl() { return m_strAccessUrl; }
	int		getLoadAllocated() { return m_nLoadAllocted; }

	int		getLastUpdate() { return m_nLastUpdate; }
	int		getScore() { return (m_nLoad==0)?100:m_nLoadMax/(m_nLoad); }

private:
	std::string	m_strName;
	std::string m_strIp;
	short	m_nPort;
	int		m_nIsp;
	int		m_nArea;
	int		m_nLoad;
	int		m_nLoadMax;
	std::string m_strAccessUrl;
	int		m_nLoadAllocted;
	int		m_nLastUpdate;
};

#endif
