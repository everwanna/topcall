#ifndef _NETPULLER_H_
#define _NETPULLER_H_

#include "config.h"

class NetPuller {
public:
	NetPuller(const std::string& mongo_ip, short mongo_port, short port);

public:
	void	run();
	void	send(const char* data, int len);
	void	pull();

private:
	int		m_nListener;
	int		m_nSocket;
	short	m_nPort;
	char*	m_pBuffer;
	std::string	m_strTopic;
	std::string		m_strMongoIp;
	short			m_nMongoPort;
	
};


#endif
