#ifndef _NETPULLER_H_
#define _NETPULLER_H_

#include "config.h"

class PullMgr;
class NetPuller {
public:
	NetPuller(PullMgr* mgr, 
		const std::string& router_ip, short router_port,
		const std::string& mongo_ip, short mongo_port
		);

public:
	void		connect();
	void		run();
	void		send(const char* data, int len);
	void		pull();

private:
	PullMgr*	m_pMgr;	
	bool		m_bConnected;
	int			m_nSocket;
	char*		m_pBuffer;
	std::string m_strRouterIp;
	short		m_nRouterPort;
	std::string	m_strTopic;
	std::string	m_strMongoIp;
	short		m_nMongoPort;
	
};


#endif
