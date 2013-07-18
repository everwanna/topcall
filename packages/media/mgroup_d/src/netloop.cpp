#include "netloop.h"
#include "mgmgr.h"
#include "msghandler.h"
#include "proxymgr.h"
#include "linkmgr.h"
#include "link.h"
#include "netlink.h"

NetLoop::NetLoop(MGMgr* mgr, const std::string& mc_ip, short mc_port, const std::string& lt_ip, short lt_port, short port) 
	: m_pMGMgr(mgr)
	, m_pMCLink(NULL)
	, m_nMPPort(port)
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		LOG(TAG_MGROUP, "ignore SIGHUP failed.");
	}
#endif
	m_event_base = event_base_new();

	//connect to mcenter:
	//m_pMCLink = new NetLink(this, "mcenter", mc_ip.c_str(), mc_port);
	//m_pMCLink->connect();

	m_pLTLink = new NetLink(this, "loadtable", lt_ip.c_str(), lt_port);
	m_pLTLink->connect();

	//startup tcp listener:
	{
		//startup tcp listener:
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		m_listener = evconnlistener_new_bind(m_event_base, mp_listener_cb, (void *)this,
			LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
			(struct sockaddr*)&sin,
			sizeof(sin));
	}

	//init the buffer:
	m_pBuffer = new char[NET_BUFFER_SIZE];

	//start a timer to do scheduler job:
	{
		timeval tm;
		tm.tv_sec = TIMER_INTERVAL;
		tm.tv_usec = 0;
		m_pTimer = event_new(m_event_base, -1, EV_READ|EV_PERSIST, timer_cb, this);
		evtimer_add(m_pTimer, &tm);
	}
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

void	NetLoop::sendMC(const char* data, int len) {
	if( m_pMCLink != NULL ) {
		m_pMCLink->send(data, len);
	}
}

void	NetLoop::sendLT(const char* data, int len) {
	if( m_pLTLink != NULL ) {
		m_pLTLink->send(data, len);
	}
}

/**
 * This function must be optimize later.
 */
int		NetLoop::getSystemTime() {
	timeval tv;
	evutil_gettimeofday(&tv, NULL);

	return tv.tv_sec;
}

void	NetLoop::mp_listener_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *user_data)
{
	sockaddr_in* addr = (sockaddr_in*)sa;	
	NetLoop *loop = (NetLoop*)user_data;
	bufferevent *bev = NULL;

	bev = bufferevent_socket_new(loop->m_event_base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		LOG(TAG_MGROUP, "NetLoop::listener_cb, fail on bufferevent_socket_new.");
		event_base_loopbreak(loop->m_event_base);
		return;
	}
	bufferevent_setcb(bev, mp_read_cb, mp_write_cb, mp_event_cb, loop);
	bufferevent_enable(bev, EV_READ|EV_WRITE);

	//add the link to the LinkMgr:
	int linkid = bufferevent_getfd(bev); 
	Link* link = new Link();
	link->linkid = linkid; 
	link->bev = bev;
	link->stamp = loop->getSystemTime();
	link->closed = false;
	loop->m_pMGMgr->getLinkMgr()->addLink(link);

	LOG(TAG_MGROUP, "connect from linkid=%d, ip/port=%s, %d", linkid, inet_ntoa(addr->sin_addr), addr->sin_port);
}

void	NetLoop::mp_read_cb(struct bufferevent *bev, void *ctx)
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
			LOG(TAG_MGROUP, "NetLoop::read_cb, len<=4, len=%d", len);	
			break;
		}

		//evbuffer_remove(src, &msg_len, 4);
		evbuffer_copyout(src, &lenbuf, 4);
		msg_len = lenbuf[0]<<24|lenbuf[1]<<16 | lenbuf[2]>>8 | lenbuf[3];
		if( msg_len <= len ) {			
			evbuffer_remove(src, loop->m_pBuffer, msg_len);

			loop->m_pMGMgr->getHandler()->handle(bufferevent_getfd(bev), loop->m_pBuffer, msg_len);	
			if( msg_len == len ) {
				break;
			}
		} else {
			//not enough data again, return.
			LOG(TAG_MGROUP, "NetLoop::read_cb, not enough data, waiting. msg_len=%d, len=%d", msg_len, len);	
			break;
		}
	}
}

void	NetLoop::mp_write_cb(struct bufferevent *, void *)
{
	//LOG(TAG_MCENTER, "NetLoop::write_cb");	
}

void	NetLoop::mp_event_cb(bufferevent *bev, short events, void *user_data) 
{
	NetLoop* loop = (NetLoop*)user_data;
	int linkid = bufferevent_getfd(bev);

	if (events & BEV_EVENT_EOF || events & BEV_EVENT_ERROR) {
		LOG(TAG_MGROUP, "proxy disconnected, linkid=%d", linkid);
		loop->m_pMGMgr->getLinkMgr()->remove(linkid);

	} else if ( events & BEV_EVENT_CONNECTED ) {
		LOG(TAG_MGROUP, "proxy connected, linkid=%d", linkid);		
	}
}

void	NetLoop::timer_cb(int fd, short event, void* arg ) {
	//LOG("NetLoopUdp::timer_cb.");

	NetLoop* loop = (NetLoop*)arg;	
	loop->m_pMGMgr->onTimer();
}
