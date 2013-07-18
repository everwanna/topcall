#include "proxyinfo.h"

ProxyInfo::ProxyInfo(int linkid, const std::string& name, const std::string& ip, short port, bool udp) 
	: m_nLinkId(linkid)
	, m_strName(name)
	, m_strIp(ip)
	, m_nPort(port)
	, m_bUdp(udp)
{
	m_nLoad = 0;
	m_nLoadMax = 0;
}

ProxyInfo::~ProxyInfo() {
}

void	ProxyInfo::setLoad(int load) {
	m_nLoad = load;
}

void	ProxyInfo::setLoadMax(int max) {
	m_nLoadMax = max;
}

bool	ProxyInfo::isSame(const ProxyInfo& other) {
	if( (m_strIp == other.m_strIp) &&
		m_nPort == other.m_nPort ) 
		return true;

	return false;
}

bool	operator==(const ProxyInfo& info1, const ProxyInfo& info2) {
	if( info1.m_strName != info2.m_strName ||
		info1.m_strIp != info2.m_strIp ||
		info1.m_nPort != info2.m_nPort ||
		info1.m_bUdp != info2.m_bUdp || 
		info1.m_nLoad != info2.m_nLoad ||
		info1.m_nLoadMax != info2.m_nLoadMax ) {
		return false;
	}

	return true;
}
