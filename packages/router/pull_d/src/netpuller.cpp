#include "netpuller.h"
#include "pullmgr.h"
#include "mongolink.h"

NetPuller::NetPuller(PullMgr* mgr, 
					 const std::string& router_ip, short router_port,
					 const std::string& mongo_ip, short mongo_port
					) 
	: m_pMgr(mgr)
	, m_nSocket(0)
	, m_strRouterIp(router_ip)
	, m_nRouterPort(router_port)
	, m_bConnected(false)
{	
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return (1);
#endif
	m_pBuffer = new char[NET_BUFFER_SIZE];
}

void	NetPuller::connect() {
	if( m_bConnected )
		return;

	struct sockaddr_in addr;		
	LOG(TAG_PULL, "connect to router at ip=%s, port=%d.", m_strRouterIp.c_str(), m_nRouterPort);
	m_nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nRouterPort);
	addr.sin_addr.s_addr = inet_addr( m_strRouterIp.c_str() );
	if( ::connect(m_nSocket, (const sockaddr*)&addr, sizeof(addr) ) == -1 ) {
		LOG(TAG_PULL, "connect to router failed, router ip=%s, port=%d.", m_strRouterIp.c_str(), m_nRouterPort);
		return;
	}
	m_bConnected = true;
}

void	NetPuller::run() {	
	if( !m_bConnected ) {
		connect();
	}
	while(true) {
		pull();
	}
}

void	NetPuller::send(const char* data, int len) {
	if( m_nSocket == -1 ) {
		LOG(TAG_PULL, "send with m_nSocket==-1");
		return;
	}

	if( ::send(m_nSocket, data, len, 0) == -1 ) {
		::closesocket(m_nSocket);
		connect();
		send(data, len);
	}

}

void	NetPuller::pull() {
	std::string msg = m_pMgr->getMongo()->fetch();
	::send(m_nSocket, msg.c_str(), msg.length(), 0);
}


