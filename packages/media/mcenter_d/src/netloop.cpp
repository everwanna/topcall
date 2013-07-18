#include "netloop.h"
#include "mcmgr.h"
#include "msghandler.h"
#include "link.h"
#include "linkmgr.h"

NetLoop::NetLoop(MCMgr* mgr, const char* dm_ip, short dm_port, short port) 
	: m_pMCMgr(mgr)
	, m_strDMIp(dm_ip)
	, m_nDMPort(dm_port)
	, m_nPort(port)
	, m_pDMReconnTimer(NULL)
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		LOG(TAG_MCENTER, "ignore SIGHUP failed.");
	}
#endif
	m_event_base = event_base_new();

	connectDM();

	//startup tcp listener:
	{
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		m_listener = evconnlistener_new_bind(m_event_base, tcp_listener_cb, (void *)this,
			LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
			(struct sockaddr*)&sin,
			sizeof(sin));
	}

	//init the buffer:
	m_pBuffer = new char[NET_BUFFER_SIZE];

	//init the timer:
	timeval tm;
	tm.tv_sec = TIMER_INTERVAL;
	tm.tv_usec = 0;
	m_pTimer = event_new(m_event_base, -1, EV_READ|EV_PERSIST, timer_cb, this);
    evtimer_add(m_pTimer, &tm);
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
}

void	NetLoop::run() 
{
	event_base_dispatch(m_event_base);
}

/**
 * This function must be optimize later.
 */
int		NetLoop::getSystemTime() {
	timeval tv;
	evutil_gettimeofday(&tv, NULL);

	return tv.tv_sec;
}

void	NetLoop::connectDM() {
	LOG(TAG_MCENTER, "connect to deamon, ip/port=%s, %d", m_strDMIp.c_str(), m_nDMPort);

	memset(&m_addrDM, 0, sizeof(m_addrDM));
	m_addrDM.sin_family = AF_INET;
	m_addrDM.sin_port = htons(m_nDMPort);
	m_addrDM.sin_addr.s_addr = inet_addr(m_strDMIp.c_str());
	m_pDMLink = bufferevent_socket_new(m_event_base, -1, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(m_pDMLink, dm_read_cb, dm_write_cb, dm_event_cb, this);
	bufferevent_enable(m_pDMLink, EV_READ|EV_WRITE);

	bufferevent_socket_connect(m_pDMLink, (sockaddr*)&m_addrDM, sizeof(m_addrDM));
	LOG(TAG_MCENTER, "deamon linkid=%d", bufferevent_getfd(m_pDMLink) );
}

void	NetLoop::sendDM(const char* msg, int len) 
{
	if( m_pDMLink != NULL ) {
		bufferevent_write(m_pDMLink, msg, len);
	}
}

void	NetLoop::closeDM() {
	if( m_pDMLink != NULL ) {		
		bufferevent_free(m_pDMLink);
		m_pDMLink = NULL;
	}
}

void	NetLoop::startReconnDM() {
	//bufferevent_socket_connect( m_pMCLink, (sockaddr*)&m_addrMc, sizeof(m_addrMc));
	timeval tm;
	tm.tv_sec = RECONN_INTERVAL;
	tm.tv_usec = 0;
	m_pDMReconnTimer = event_new(m_event_base, -1, EV_READ, dm_reconn_cb, this);
	event_add(m_pDMReconnTimer, &tm);
}

void	NetLoop::stopReconnDM() {
	if( m_pDMReconnTimer ) {
		event_del(m_pDMReconnTimer);
		event_free(m_pDMReconnTimer);
		m_pDMReconnTimer = NULL;
	}
}

void	NetLoop::dm_read_cb(struct bufferevent *bev, void *ctx)
{
	static int seq = 0;
	//LOG(TAG_MGROUP, "NetLoop::read_cb, seq=%d", seq++);	
	NetLoop* loop = (NetLoop*)ctx;
	struct evbuffer *src;
	size_t len;
	char lenbuf[4];
	size_t msg_len;

	src = bufferevent_get_input(bev);	
	while(true) {
		len = evbuffer_get_length(src);	
		if( len <= 4 ) {
			LOG(TAG_MCENTER, "NetLoop::read_cb, len<=4");	
			break;
		}

		evbuffer_copyout(src, &lenbuf, 4);
		msg_len = lenbuf[0]<<24|lenbuf[1]<<16 | lenbuf[2]>>8 | lenbuf[3];
		if( msg_len <= len ) {			
			evbuffer_remove(src, loop->m_pBuffer, msg_len);

			loop->m_pMCMgr->getHandler()->handle(bufferevent_getfd(bev), loop->m_pBuffer, msg_len);	
			if( msg_len == len ) {
				break;
			}					
		} else {
			//not enough data again, return.
			LOG(TAG_MCENTER, "NetLoop::read_cb, not enough data, waiting. msg_len=%d, len=%d", msg_len, len);	
			break;
		}
	}
}

void	NetLoop::dm_write_cb(struct bufferevent *, void *)
{
	//LOG(TAG_MGROUP, "NetLoop::write_cb");	
}

void	NetLoop::dm_event_cb(bufferevent *bev, short events, void *user_data) 
{
	NetLoop* loop = (NetLoop*)user_data;
	int linkid = bufferevent_getfd(bev);

	if (events & BEV_EVENT_ERROR) {
		LOG(TAG_MCENTER, "deamon disconnected, linkid=%d", linkid);
		loop->closeDM();
		loop->stopReconnDM();
		loop->startReconnDM();
	} else if( events&BEV_EVENT_CONNECTED ) {
		LOG(TAG_MCENTER, "deamon connected, linkid=%d", linkid);
		loop->stopReconnDM();
	}
}

void	NetLoop::dm_reconn_cb(int fd, short event, void* arg) {
	NetLoop* loop = (NetLoop*)arg;
	loop->connectDM();
}

//listen for mgroup or mcproxy
void	NetLoop::tcp_listener_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *user_data)
{
	sockaddr_in* addr = (sockaddr_in*)sa;	
	NetLoop *loop = (NetLoop*)user_data;
	bufferevent *bev = NULL;

	LOG(TAG_MCENTER, "NetLoop::listener_cb, fd/ip/port=%d, %d:%d.", fd, addr->sin_addr.s_addr, addr->sin_port);
	//if( !loop->m_pMCMgr->getWhiteList()->has(addr->sin_addr.s_addr) ) {
	//	return;
	//}

	bev = bufferevent_socket_new(loop->m_event_base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		LOG(TAG_MCENTER, "NetLoop::listener_cb, fail on bufferevent_socket_new.");
		event_base_loopbreak(loop->m_event_base);
		return;
	}
	bufferevent_setcb(bev, tcp_read_cb, tcp_write_cb, tcp_event_cb, loop);
	bufferevent_enable(bev, EV_READ|EV_WRITE);

	//add the link to the LinkMgr:
	int linkid = bufferevent_getfd(bev); 
	Link* link = new Link();
	link->linkid = linkid; 
	link->bev = bev;
	link->stamp = loop->getSystemTime();
	link->closed = false;
	loop->m_pMCMgr->getLinkMgr()->addLink(link);

	LOG(TAG_MCENTER, "connect from linkid=%d", linkid);
}

void	NetLoop::tcp_read_cb(struct bufferevent *bev, void *ctx)
{
#ifdef _DEBUG
	static int seq = 0;
	//LOG(TAG_MCENTER, "NetLoop::read_cb, seq=%d", seq++);	
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
			LOG(TAG_MCENTER, "NetLoop::read_cb, len<=4, len=%d", len);	
			break;
		}

		//evbuffer_remove(src, &msg_len, 4);
		evbuffer_copyout(src, &lenbuf, 4);
		msg_len = lenbuf[0]<<24|lenbuf[1]<<16 | lenbuf[2]>>8 | lenbuf[3];
		if( msg_len <= len ) {			
			evbuffer_remove(src, loop->m_pBuffer, msg_len);

			loop->m_pMCMgr->getHandler()->handle(bufferevent_getfd(bev), loop->m_pBuffer, msg_len);	
			if( msg_len == len ) {
				break;
			}
		} else {
			//not enough data again, return.
			LOG(TAG_MCENTER, "NetLoop::read_cb, not enough data, waiting. msg_len=%d, len=%d", msg_len, len);	
			break;
		}
	}
}

void	NetLoop::tcp_write_cb(struct bufferevent *, void *)
{
	//LOG(TAG_MCENTER, "NetLoop::write_cb");	
}

void	NetLoop::tcp_event_cb(bufferevent *bev, short events, void *user_data) 
{
	NetLoop* loop = (NetLoop*)user_data;
	int linkid = bufferevent_getfd(bev);
	
	if (events & BEV_EVENT_ERROR || events & BEV_EVENT_EOF ) {
		LOG(TAG_MCENTER, "link disconnected, linkid=%d", linkid);
		loop->m_pMCMgr->getLinkMgr()->remove(linkid);
	} else if( events & BEV_EVENT_CONNECTED ) {
		LOG(TAG_MCENTER, "link connected, linkid=%d", linkid);
	}
}

void	NetLoop::timer_cb(int fd, short event, void* arg ) {
	//LOG("NetLoopUdp::timer_cb.");

	NetLoop* loop = (NetLoop*)arg;	
	loop->m_pMCMgr->onTimer();
}

