#ifndef _PROXYINFO_H_
#define _PROXYINFO_H_

#include "config.h"

class ProxyInfo {
public:
	ProxyInfo(int linkid, const std::string& name, const std::string& ip, short port, bool udp);
	~ProxyInfo();

public:
	void	setLoad(int load);
	void	setLoadMax(int max);

public:
	int		getLinkId() { return m_nLinkId; }
	bool	isSame(const ProxyInfo& other) ;
	std::string getName() { return m_strName; }
	std::string getIp() { return m_strIp; }
	short	getPort() { return m_nPort; }
	bool	getUdp() { return m_bUdp; }
	int		getLoad() { return m_nLoad; }
	int		getLoadMax() { return m_nLoadMax; }

	friend bool	operator==(const ProxyInfo& info1, const ProxyInfo& info2);

private:
	int		m_nLinkId;
	std::string		m_strName;
	std::string		m_strIp;
	short	m_nPort;
	bool	m_bUdp;
	int		m_nLoad;
	int		m_nLoadMax;
};

#endif
