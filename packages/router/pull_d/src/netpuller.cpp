#include "netpuller.h"

NetPuller::NetPuller(const std::string& mongo_ip, short mongo_port, short port) 
	: m_nListener(0)
	, m_nSocket(0)
	, m_nPort(port)
{	
	m_pBuffer = new char[NET_BUFFER_SIZE];
}

void	NetPuller::run() {
	struct sockaddr_in addr;
	struct sockaddr_in peer;
	int peer_len = sizeof(peer);

	LOG(TAG_PULL, "start listener on port %d.", m_nPort);
	m_nListener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPort);
	if( bind(m_nListener, (struct sockaddr*)&addr, sizeof(addr)) == -1 ) {
		LOG(TAG_PULL, "listen to port failed, port=%d", m_nPort);
		return;
	}

	if( listen(m_nListener, 1 ) == -1 ) {
		LOG(TAG_PULL, "listner failed.");
		return;
	}

	memset(&peer, 0, sizeof(peer));
	m_nSocket = accept(m_nListener, (sockaddr*)&peer, &peer_len);
	if( m_nSocket == -1 ) {
		LOG(TAG_PULL, "accept socket==-1.");
		return;
	}
	LOG(TAG_PULL, "accept on port %d, start pulling...", m_nPort);

	while(true) {
		int len = recv(m_nSocket, m_pBuffer, NET_BUFFER_SIZE, 0);	
		int msg_len;
		if( len > 4 ) {
			msg_len = m_pBuffer[0]<<24|m_pBuffer[1]<<16 | m_pBuffer[2]>>8 | m_pBuffer[3];
			if( msg_len <= len ) {
				Unpack up(m_pBuffer, len);
				up.popHead();

				if( up.getUri() != URI_CONSUMER_REGISTER_REQ ) {
					LOG(TAG_PULL, "the packet is not PConsumerRegister.");
					continue;
				} else {
					PConsumerRegisterReq reg;
					reg.unmarshall(up);
					m_strTopic = reg.topic;
					break;
				}
			}
		} else {
			LOG(TAG_PULL, "read invalid package, len<4, len=%d", len);
			return;
		}
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

	::send(m_nSocket, data, len, 0);
}

void	NetPuller::pull() {
	
}


