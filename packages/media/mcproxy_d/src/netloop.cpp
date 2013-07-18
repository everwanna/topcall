#include "netloop.h"
#include "mcpmgr.h"
#include "msghandler.h"

NetLoop::NetLoop(MCPMgr* mgr, const char* mc_ip, short mc_port, short port) 
	: m_pMCPMgr(mgr)
	, m_pMCLink(NULL)
	, m_strMCIp(mc_ip)
	, m_nMCPort(mc_port)
	, m_pMCReconnTimer(NULL)
	, m_nPort(port)
	, m_pingPack(SVID_MCENTER, PMCPing::uri)
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		LOG(TAG_MCPROXY, "ignore SIGHUP failed.");
	}
#endif
	m_event_base = event_base_new();

	//startup tcp listener:
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	m_listener = evconnlistener_new_bind(m_event_base, tcp_listener_cb, (void *)this,
	    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
	    (struct sockaddr*)&sin,
	    sizeof(sin));
	LOG(TAG_MCPROXY, "mcproxy listen on port=%d.", port);

	//init the buffer:
	m_pBuffer = new char[NET_BUFFER_SIZE];

	//connect to media center:
	connectMC(mc_ip, mc_port);

	//init the timer:
	timeval tm;
	tm.tv_sec = TIMER_INTERVAL;
	tm.tv_usec = 0;
	m_pTimer = event_new(m_event_base, -1, EV_READ|EV_PERSIST, timer_cb, this);
    evtimer_add(m_pTimer, &tm);

	PMCPing ping;
	ping.marshall(m_pingPack);
	m_pingPack.pack();
}

NetLoop::~NetLoop()
{
	if( m_event_base )
	{
		event_base_free(m_event_base);
		m_event_base = NULL;
	}
	if( m_pBuffer ) {
		delete[] m_pBuffer;
	}
	//[TBD] mcenter:
}

void	NetLoop::run() 
{
	event_base_dispatch(m_event_base);
}

void	NetLoop::send(int linkid, const char* msg, int len)
{
	std::map<int, bufferevent*>::iterator it = m_mapLinks.find(linkid);
	if( it == m_mapLinks.end() ) {
		return;
	}

	bufferevent_write(it->second, msg, len);
	//bufferevent_flush(it->second, EV_WRITE, BEV_NORMAL);
}

void	NetLoop::remove(int linkid) {
	std::map<int, bufferevent*>::iterator it = m_mapLinks.find(linkid);
	if( it == m_mapLinks.end() ) {
		return;
	}

	bufferevent_free(it->second);
	m_mapLinks.erase(it);
}

void	NetLoop::connectMC(const char* mc_ip, short mc_port) {
	LOG(TAG_MCPROXY, "connect to mcenter, mc_ip/mc_port= %s, %d", mc_ip, (int)mc_port);

	memset(&m_addrMc, 0, sizeof(m_addrMc));
	m_addrMc.sin_family = AF_INET;
	m_addrMc.sin_port = htons(m_nMCPort);
	m_addrMc.sin_addr.s_addr = inet_addr(m_strMCIp.c_str());
	m_pMCLink = bufferevent_socket_new(m_event_base, -1, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(m_pMCLink, mc_read_cb, NULL, mc_event_cb, this);
	bufferevent_enable(m_pMCLink, EV_READ);
	bufferevent_socket_connect(m_pMCLink, (sockaddr*)&m_addrMc, sizeof(m_addrMc));
	LOG(TAG_MCPROXY, "mecenter link id=%d", bufferevent_getfd(m_pMCLink) );
}

void	NetLoop::sendMC(char *msg, int len) {
	if( NULL != m_pMCLink ) {
		bufferevent_write(m_pMCLink, msg, len);
	}
}

void	NetLoop::closeMC() {
	if( m_pMCLink != NULL ) {		
		bufferevent_free(m_pMCLink);
		m_pMCLink = NULL;
	}
}


void	NetLoop::startReconnMC() {
	timeval tm;
	tm.tv_sec = MC_RECONN_INTERVAL;
	tm.tv_usec = 0;
	m_pMCReconnTimer = event_new(m_event_base, -1, EV_READ, mc_reconn_cb, this);
	event_add(m_pMCReconnTimer, &tm);
}

void	NetLoop::stopReconnMC() {
	if( m_pMCReconnTimer ) {
		event_del(m_pMCReconnTimer);
		event_free(m_pMCReconnTimer);
		m_pMCReconnTimer = NULL;
	}
}

int		NetLoop::getSystemTime() {
	timeval tv;
	evutil_gettimeofday(&tv, NULL);

	return tv.tv_sec;
}

void	NetLoop::tcp_listener_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *user_data)
{
	sockaddr_in* addr = (sockaddr_in*)sa;	
	NetLoop *loop = (NetLoop*)user_data;
	bufferevent *bev = NULL;

	//LOG(TAG_MCPROXY, "NetLoop::listener_cb, fd/ip/port=%d, %d:%d.", fd, addr->sin_addr.s_addr, addr->sin_port);
	bev = bufferevent_socket_new(loop->m_event_base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		LOG(TAG_MCPROXY, "NetLoop::listener_cb, fail on bufferevent_socket_new.");
		event_base_loopbreak(loop->m_event_base);
		return;
	}
	bufferevent_setcb(bev, tcp_read_cb, NULL, tcp_event_cb, loop);	
	bufferevent_enable(bev, EV_READ);

	int linkid = bufferevent_getfd(bev); 
	loop->m_mapLinks[linkid] = bev;
}

void	NetLoop::tcp_read_cb(struct bufferevent *bev, void *ctx)
{
#ifdef _DEBUG
	static int seq = 0;
	//LOG(TAG_MCPROXY, "NetLoop::read_cb, seq=%d", seq++);	
#endif

	NetLoop* loop = (NetLoop*)ctx;
	struct evbuffer *src;
	size_t len;
	char lenbuf[4];
	size_t msg_len;

	src = bufferevent_get_input(bev);	
	while(true) {
		len = evbuffer_get_length(src);	
		if( len <= 4 ) {
			LOG(TAG_MCPROXY, "NetLoop::read_cb, len<=4, len=%d", len);	
			break;
		}

		//evbuffer_remove(src, &msg_len, 4);
		evbuffer_copyout(src, &lenbuf, 4);
		msg_len = lenbuf[0]<<24|lenbuf[1]<<16 | lenbuf[2]>>8 | lenbuf[3];
		if( msg_len <= len ) {			
			evbuffer_remove(src, loop->m_pBuffer, msg_len);

			loop->m_pMCPMgr->getHandler()->handle(bufferevent_getfd(bev), loop->m_pBuffer, msg_len);	
			if( msg_len == len ) {
				break;
			}
		} else {
			//not enough data again, return.
			LOG(TAG_MCPROXY, "NetLoop::read_cb, not enough data, waiting. msg_len=%d, len=%d", msg_len, len);	
			break;
		}
	}
}

void	NetLoop::tcp_write_cb(struct bufferevent *, void *)
{
	//LOG(TAG_MCPROXY, "NetLoop::write_cb");	
}

void	NetLoop::tcp_event_cb(bufferevent *bev, short events, void *user_data) 
{
	NetLoop* loop = (NetLoop*)user_data;
	int linkid = bufferevent_getfd(bev);

	/*
	if (events & BEV_EVENT_EOF) {
		LOG(TAG_MCPROXY, "NetLoop::event_cb, BEV_EVENT_EOF for linkid %d", linkid);
		loop->remove(linkid);

	} else 
	*/
	if (events & BEV_EVENT_ERROR) {
		LOG(TAG_MCPROXY, "link error for linkid %d", linkid);
		loop->remove(linkid);
	}
}

void	NetLoop::timer_cb(int fd, short event, void* arg ) {
	//LOG("NetLoopUdp::timer_cb.");

	//send a ping to mcenter first:
	NetLoop* loop = (NetLoop*)arg;	
	loop->sendMC( loop->m_pingPack.getBuf(), loop->m_pingPack.getLen());
	loop->m_pMCPMgr->onTimer();
}

//mc link:
void	NetLoop::mc_read_cb(struct bufferevent *bev, void *ctx)
{
#ifdef _DEBUG
	static int seq = 0;
	//LOG(TAG_MCPROXY, "NetLoop::mc_read_cb, seq=%d", seq++);	
#endif

	NetLoop* loop = (NetLoop*)ctx;
	struct evbuffer *src;
	size_t len;
	char lenbuf[4];
	size_t msg_len;

	src = bufferevent_get_input(bev);	
	while(true) {
		len = evbuffer_get_length(src);	
		if( len <= 4 ) {
			LOG(TAG_MCPROXY, "NetLoop::mc_read_cb, len<=4, len=%d", len);	
			break;
		}

		//evbuffer_remove(src, &msg_len, 4);
		evbuffer_copyout(src, &lenbuf, 4);
		msg_len = lenbuf[0]<<24|lenbuf[1]<<16 | lenbuf[2]>>8 | lenbuf[3];
		if( msg_len <= len ) {			
			evbuffer_remove(src, loop->m_pBuffer, msg_len);

			loop->m_pMCPMgr->getHandler()->handle(bufferevent_getfd(bev), loop->m_pBuffer, msg_len);	
			if( msg_len == len ) {
				break;
			}
		} else {
			//not enough data again, return.
			LOG(TAG_MCPROXY, "NetLoop::mc_read_cb, not enough data, waiting. msg_len=%d, len=%d", msg_len, len);	
			break;
		}
	}
}

void	NetLoop::mc_write_cb(struct bufferevent *, void *)
{
	//LOG(TAG_MCPROXY, "NetLoop::mc_write_cb");	
}

void	NetLoop::mc_event_cb(bufferevent *bev, short events, void *user_data) 
{
	NetLoop* loop = (NetLoop*)user_data;
	int linkid = bufferevent_getfd(bev);
	
	if (events & BEV_EVENT_ERROR ||
		events & BEV_EVENT_EOF
		) {
		LOG(TAG_MCPROXY, "mcenter disconnected, linkid %d", linkid);
		loop->closeMC();
		loop->stopReconnMC();
		loop->startReconnMC();

	} else if( events&BEV_EVENT_CONNECTED ) {
		LOG(TAG_MCPROXY, "mcenter connected, linkid %d", linkid);
		loop->stopReconnMC();
	}
}

void	NetLoop::mc_reconn_cb(int fd, short event, void* arg) {
	NetLoop* loop = (NetLoop*)arg;
	loop->connectMC(loop->m_strMCIp.c_str(), loop->m_nMCPort);
}
