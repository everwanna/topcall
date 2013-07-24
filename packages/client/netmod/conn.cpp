#include "conn.h"
#include "ioengine.h"
#include "linklayerproxy.h"
#include "mempool.h"
#include <MMSystem.h>

#if defined(__i386__)||defined(_WIN32) || defined(__x86_64__) //big start
#define XHTONS
#define XHTONL
#define XHTONLL

#else /* big end */
inline uint16_t XHTONS(uint16_t i16)
{
    return((i16 << 8) | (i16 >> 8));
}
inline uint32_t XHTONL(uint32_t i32)
{
    return((uint32_t(XHTONS(i32)) << 16) | XHTONS(i32>>16));
}
inline uint64_t XHTONLL(uint64_t i64)
{
    return((uint64_t(XHTONL((uint32_t)i64)) << 32) |XHTONL((uint32_t(i64>>32))));
}
#endif /* __i386__ */


static uint32_t g_uLastTime = 0;

CConn::CConn()
: m_sockfd(INVALID_SOCKET)
, m_sockType(SOCK_STREAM)
, m_status(CONN_INIT)
, m_pEvH(NULL)
, m_pFirstLayer(NULL)
, m_pLastLayer(NULL)
{
    m_connId = ATOMIC_ADD(&g_curConnId);
    m_localAddr.sin_family		= AF_INET;
    m_localAddr.sin_port		= htons(0);
    m_localAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
    m_remoteAddr.sin_family		= AF_INET;
    m_remoteAddr.sin_port		= htons(0);
    m_remoteAddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

CConn::~CConn()
{
    ILinkLayer * layer = m_pFirstLayer;
    while (layer)
    {
        m_pFirstLayer = m_pFirstLayer->m_pNextLayer;
        delete layer;
        layer = m_pFirstLayer;
    }
}

int CConn::init(ConnAttr* attr)
{
    if ( ConnAttr::CONN_TCP == attr->ConnType )
        m_sockType = SOCK_STREAM;
    else if ( ConnAttr::CONN_UDP == attr->ConnType )
        m_sockType = SOCK_DGRAM;
    else
        //unknow protocol
        return -1;

    m_sockfd = ::socket(AF_INET, m_sockType, 0);
    int on = 0;
#ifdef _WIN32
    ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
#else
    ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)); //forbid reuse port!!! cause reuse last port may trigger onSend event(but connection closed last time)
#endif
    if ( INVALID_SOCKET == m_sockfd )
        return -1;
    
    setNBlock(); //set this socket non-block

    if (attr->LocalIP != INADDR_ANY)
        m_localAddr.sin_addr.s_addr = attr->LocalIP;
    if (attr->LocalPort != 0) 
        m_localAddr.sin_port = htons(attr->LocalPort);

    if (attr->RemoteIP != INADDR_ANY)
        m_remoteAddr.sin_addr.s_addr = attr->RemoteIP;
    if (attr->RemotePort != 0)
        m_remoteAddr.sin_port = htons(attr->RemotePort);

    if (attr->evHandler)
        m_pEvH = attr->evHandler;

    // create layers
    ILinkLayer* layer = NULL;
    Extension** ext = attr->exts;
    for (int i = 0; ext[i] != NULL; i++) 
    {
        // Create Each Layer and link them together
        layer = createLayer(ext[i]);
        if (layer) 
        {
            layer->m_pOwner = this;
            if (m_pFirstLayer == NULL) {
                layer->m_pNextLayer = layer->m_pPreLayer = NULL;
                m_pFirstLayer = m_pLastLayer = layer;
            }
            else {
                layer->m_pPreLayer = m_pLastLayer;
                m_pLastLayer->m_pNextLayer = layer;
                m_pLastLayer = layer;
            }
        }
    }
    return m_connId;
}

int CConn::connect(uint32_t ip, uint16_t port)
{    
	NET_LOG("CConn::connect, connId/ip/port=", m_connId, ip, port);
    m_status = CONN_CONNECTING;

	if ( INADDR_ANY != ip )
		m_remoteAddr.sin_addr.s_addr = ip;
	if ( 0 != port)
		m_remoteAddr.sin_port = htons(port);

    if (m_pFirstLayer)
        return m_pFirstLayer->connect(m_remoteAddr.sin_addr.s_addr, m_remoteAddr.sin_port, m_sockType);
    else
        return _connect(m_remoteAddr.sin_addr.s_addr, m_remoteAddr.sin_port, m_sockType);
}

int CConn::_connect(uint32_t ip, uint16_t port, int sockType)
{
    IoEngine::Instance()->setEvent(this, m_sockfd, FD_OP_READ | FD_OP_WRITE);

    sockaddr_in destAddr; //ip and port couldn't be 0, coz they were inited in connect
    destAddr.sin_family = AF_INET;
    destAddr.sin_addr.s_addr = ip;
    destAddr.sin_port = port;

    notifyConnState(CNetEventConnState::CS_TRANSPTLAYER_CONNECTING);

    if ( SOCK_STREAM == sockType )
    {
        if ( ::connect(m_sockfd, (struct sockaddr*)&destAddr, sizeof(destAddr) ) == -1 )
        {
            uint32_t uLastErrNum = 0;
#ifdef _WIN32
            uLastErrNum = WSAGetLastError();
            if ( WSAEWOULDBLOCK == uLastErrNum )
                return 0;//still success
#else
            uLastErrNum = errno;
            if( EINPROGRESS == uLastErrNum )
            {
                NET_LOG("CConn::_connect, EINPROGRESS");
                return 0; //still success
            }
#endif
            NET_LOG("CConn::_connect, Error: connect failed, lastError=",uLastErrNum);
            this->onError();
            return uLastErrNum;
        }
        return 0;
    }
    else if ( SOCK_DGRAM == sockType )
    {
        int iError = bind(m_sockfd, (sockaddr*)&m_localAddr, sizeof(m_localAddr));
        if ( iError < 0 )
            return -1;

        int oplen = sizeof(int);
        int sock_bufsize = 8*1024*1024;
        if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (char*)&sock_bufsize, oplen) != 0) {
            return -1;
        }
        if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, (char*)&sock_bufsize, oplen) != 0) {
            return -1;
        }
    }
    return -1;
}

int CConn::send(char* data, size_t len)
{
	if( m_status == CONN_CLOSE || m_sockfd == INVALID_SOCKET)
		return -1;

    if (m_pFirstLayer)
        return m_pFirstLayer->send(data ,len);
    else
        return _send(data, len);
}

int CConn::_send(char* data, size_t len)
{
    int ret = 0;
    sockaddr_in* pAddr = NULL;
    if ( SOCK_DGRAM == m_sockType )
        pAddr = &m_remoteAddr;
        
    ret = m_output.write(m_sockfd, data, len, pAddr, m_sockType);
    if (!m_output.empty()) //if means that socket can't send more data just now(maybe block),notify select to tell us when to send another data still in buffer
        IoEngine::Instance()->setEvent(this, m_sockfd, FD_OP_WRITE);
    return ret;
}

int CConn::close()
{    
	int ret = -1;

	if( m_sockfd != INVALID_SOCKET )
	{
		NET_LOG("CConn::close, m_connId/socket=", m_connId, m_sockfd);
	}
    if (m_pFirstLayer)
        ret = m_pFirstLayer->close();
    else
        ret = _close();

	return ret;
}

int CConn::_close()
{
    m_status = CONN_CLOSE;
	m_pEvH = NULL;
	if( m_sockfd != INVALID_SOCKET )
	{
		IoEngine::Instance()->setEvent(this, m_sockfd, FD_OP_CLR);
	}

	if( m_sockfd != INVALID_SOCKET )
	{
#ifdef _WIN32
        ::closesocket(m_sockfd);
#else
        ::close(m_sockfd);
#endif
		m_sockfd = INVALID_SOCKET;
	}
    return 0;
}

int CConn::onRecv()
{	
    if (CONN_CLOSE == m_status || m_sockfd == INVALID_SOCKET )
        return -1;

    if ( CONN_CONNECTING == m_status )
    {
        onConnected();
        m_status = CONN_CONNECTED;
        return 0;
    }

    sockaddr_in  srvAddr;
    sockaddr_in* pAddr = NULL;
    if ( SOCK_DGRAM == m_sockType )
        pAddr = &srvAddr;

    int nrecv = m_input.read(m_sockfd, pAddr, m_sockType); //in UDP,pAddr point to where data from, maybe we should compare it with m_remoteAddr.anyway, let it go now
    if ( nrecv > 0)
    {
        if ( m_sockType == SOCK_DGRAM )
        {
            if ( pAddr->sin_addr.s_addr != m_remoteAddr.sin_addr.s_addr )
            {
                NET_LOG("Exp: udp packet not from server!!! from/server=",pAddr->sin_addr.s_addr, m_remoteAddr.sin_addr.s_addr);
            }
        }
        if (m_pLastLayer)
            m_pLastLayer->onData( m_input, nrecv ); //handle the data that received just now, not all data in buffer
        else
            _onData();
    }
    else
    {
        //connection maybe broken!
        NET_LOG("call recv(recvfrom) but read nothing, connection maybe reset by peer, connId/sockType/nrecv=",m_connId,m_sockType,nrecv);
        return onError();
    }
    return -1;
}

int CConn::_onMsgOOB(char* data, size_t len)    //MSG_OOB, urgent channel
{
    if(m_sockType != SOCK_STREAM || len < 4 || data == NULL)
    {
        return -1;
    }

    uint32_t length = peeklen(data);
    if(length <= 4 || len < length)
    {
        NET_LOG("_onMsgOOB!!!wrong length of a MSG_OOB packet!!!len/buf_size=",length, len);
        return -1;
    }

    CNetEvent evt;
    Packet* pkt = MemPool::Instance()->newPacket(data, length);
#ifdef _WIN32
    pkt->_timestamp = getCurTime();
#else
    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    uint32_t t = (curTime.tv_sec * 1000) + (curTime.tv_usec / 1000);
    pkt->_timestamp = t;
#endif
    if ( m_pEvH )
    {
        evt.EvtType = CNetEvent::EV_IN;
        evt.RetVal = 0;
        m_pEvH->OnEvent(&evt, pkt);
    }  

    return 0;
}

int CConn::_onData()
{
    CNetEvent evt;
    if ( SOCK_STREAM == m_sockType)
    {
        //assemble packet
        while (!m_input.empty())
        {
            if (m_input.size() < 4) //length < header, not enough data, tcp wait
                break;

            uint32_t length = peeklen(m_input.data()); //let's see how long the packet is
            if (length <= 4)
            {
                NET_LOG("TCP Fxxk!!!wrong length of a packet!!!len/buf_size=",length, m_input.size());
                onError();
            }

            if (m_input.size() < length) //current data in buffer can't assemble a packet, not enough data
                break;

            Packet* pkt = MemPool::Instance()->newPacket(m_input.data(), length);
#ifdef _WIN32
            pkt->_timestamp = getCurTime();
#else
            struct timeval curTime;
            gettimeofday(&curTime, NULL);
            uint32_t t = (curTime.tv_sec * 1000) + (curTime.tv_usec / 1000);
            pkt->_timestamp = t;
#endif
            if ( m_pEvH )
            {
                evt.EvtType = CNetEvent::EV_IN;
                evt.RetVal = 0;
                m_pEvH->OnEvent(&evt, pkt);
            }

            m_input.erase(0, length);
        }
    }
    else if ( SOCK_DGRAM == m_sockType )
    {
        if ( m_input.size() < 4 ) //length < header, not enough data, udp drop this unnormal packet
        {
            NET_LOG("UDP Error: udp packet size < 4, drop it. size=",m_input.size());
            m_input.free();
            return 0;
        }

        uint32_t length = peeklen(m_input.data()); //let's see how long the packet is
        if ( length != m_input.size() ) //an udp packet in input buffer
        {
            NET_LOG("UDP Fxxk!!!wrong length of a packet!!!len/buf_size=",length,m_input.size());
            m_input.free();
            return 0;
        }

        Packet* pkt = MemPool::Instance()->newPacket(m_input.data(), length);
#ifdef _WIN32
        pkt->_timestamp = getCurTime();
#else
        struct timeval curTime;
        gettimeofday(&curTime, NULL);
        uint32_t t = (curTime.tv_sec * 1000) + (curTime.tv_usec / 1000);
        pkt->_timestamp = t;
#endif
        if ( m_pEvH )
        {
            evt.EvtType = CNetEvent::EV_IN;
            evt.RetVal = 0;
            m_pEvH->OnEvent(&evt, pkt);
        }
        m_input.erase(0, length);
    }
    
    return 0;
}

int CConn::onSend()
{
    if ( CONN_CLOSE == m_status || m_sockfd == INVALID_SOCKET )
        return -1;

    if ( CONN_CONNECTING == m_status )
    {
        onConnected();
        m_status = CONN_CONNECTED;
        return 0;
    }    
	
    if (m_pLastLayer)
        return m_pLastLayer->onSend();
    else
        return _onSend();
}

int CConn::_onSend()
{
	if ( CONN_CLOSE == m_status || m_sockfd == INVALID_SOCKET )
		return -1;

    if (m_pEvH) 
    {
        CNetEvent evt;
        evt.ConnId	= m_connId;
        evt.EvtType = CNetEvent::EV_SENT;
        m_pEvH->OnEvent(&evt, NULL);
    }

    sockaddr_in* pAddr = NULL;
    if (SOCK_DGRAM == m_sockType)
        pAddr = &m_remoteAddr;

    m_output.flush(m_sockfd, pAddr, m_sockType);
    if (m_output.empty()) //if data in buffer send over, unregister onWrite event from select (or onSend event will be given every time!)
        IoEngine::Instance()->setEvent(this, m_sockfd, FD_OP_WRITE, false); //unregister onWrite event

    return 0;
}

int CConn::onConnected()
{    
	NET_LOG("CConn::onConnected, m_connId/socket/status=", m_connId, m_sockfd, m_status);

    IoEngine::Instance()->setEvent(this, m_sockfd, FD_OP_WRITE, false); //unregister onWrite event from select (or onSend event will be given every time!)
    
    //stats
    notifyConnState(CNetEventConnState::CS_TRANSPTLAYER_CONNECTED);

    if (m_pLastLayer)
        return m_pLastLayer->onConnected();
    else
        return _onConnected();
}

int CConn::_onConnected()
{
    if ( m_pEvH ) 
    {
        CNetEvent evt;
        evt.ConnId	= m_connId;
        evt.EvtType = CNetEvent::EV_CONNECTED;
        evt.RetVal	= 0;
        m_pEvH->OnEvent(&evt, NULL);

        //stats
        notifyConnState(CNetEventConnState::CS_CONNECTED);
    }

    return 0;
}

int CConn::onError()
{    
	NET_LOG("CConn::onError, m_connId/socket/status=", m_connId, m_sockfd, m_status);

    IoEngine::Instance()->setEvent(this, m_sockfd, FD_OP_CLR);
    if (m_pLastLayer)
        return m_pLastLayer->onError();
    else
        return _onError();
}

int CConn::_onError()
{
    if ( m_pEvH )
    {
        CNetEvent evt;
        evt.ConnId	= m_connId;
        evt.EvtType = CNetEvent::EV_ERROR;
        evt.RetVal	= 0;//TO DO : nError;
        m_pEvH->OnEvent(&evt, NULL);
    }
    return 0;
}

void CConn::setNBlock()
{
#ifdef unix
    int fflags = fcntl(m_sockfd, F_GETFL);
    if (-1 == fflags)
    {
        //log(Error, "set NBlock error, socket id:%d", m_sockfd);
        return ;
    }
    fflags |= O_NONBLOCK;
    fcntl(m_sockfd, F_SETFL, fflags);
#endif

#ifdef _WIN32
    unsigned long iMode = 1;  //non-blocking mode is enabled.
    if (SOCKET_ERROR == ioctlsocket(m_sockfd, FIONBIO, &iMode) ) //set it non-block in win32
	{
        //std::cout << "CConn::setNBlock failed" << std::endl;
		NET_LOG("CConn::setNBlock, failed.");
	}
#endif
}

uint32_t CConn::getCurTime()
{
    uint32_t t = ::timeGetTime();
    if ( t == 0 )
        t = g_uLastTime;
    g_uLastTime = t;
    return t;
}

uint32_t CConn::peeklen(const void* d)
{
    uint32_t l = XHTONL( *((uint32_t*)d) );
    uint32_t len = 0;
    if ( (l & 0x80000000) == 0 )//old packet
    {
        len = l;
    }
    else //new audio packet
    {
        len = (l & 0x0000FFF0) >> 4;
    }
    return len;
}

ILinkLayer* CConn::createLayer(Extension* ext)
{
    ILinkLayer* layer = NULL;
    switch (ext->extID) 
    {
    case ExtEncryption::EXTID:
        {
            break;
        }
    case ExtProxy::EXTID:
        {
            layer = new LinkLayerProxy();
            layer->init(ext);
            break;
        }
    default:
        return NULL;
    }
    return layer;
}

uint32_t CConn::tryPartitionPkt()
{
    if ( SOCK_STREAM == m_sockType && m_input.size() >= 4)
    {
        uint32_t length = peeklen(m_input.data()); //let's see how long the packet is
        if (length <= 4)
        {
            NET_LOG("tryPartitionPkt: wrong length of a packet!!!len/buf_size=",length, m_input.size());
            return -1;
        }
        else if (length < m_input.size())//current data in buffer can't assemble a packet, not enough data
        {
            return 0;
        }
        else
        {
            return length;
        }
    }

    return 0;
}

void   CConn::notifyConnState(int state)
{
    CNetEventConnState evt;
    evt.EvtType = CNetEvent::EV_CONNSTATE;
    evt.ConnId = m_connId;
    evt.RetVal = 0;
    evt.state = state;
    evt.timestamp = ::GetTickCount();

    if(m_pEvH)
       m_pEvH->OnEvent(&evt, NULL);
}


