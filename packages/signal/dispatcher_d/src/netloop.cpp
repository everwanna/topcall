#include "netloop.h"
#include "dispmgr.h"
#include "msghandler.h"
#include "linkmgr.h"
#include "link.h"
#include "netlink.h"
#include "netlistener.h"

NetLoop::NetLoop(DispMgr* mgr, const std::string& router_ip, short router_port, short port) 
	: m_pMgr(mgr)
	, m_pRouterLink(NULL)
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		LOG(TAG_DISPATCHER, "ignore SIGHUP failed.");
	}
#endif
	m_event_base = event_base_new();

	//connect to master link:
	m_pRouterLink = new NetLink(this, "router_link", router_ip.c_str(), router_port);
	m_pRouterLink->connect();

	//startup tcp listener:
	m_pListener = new NetListener(this);
	m_pListener->listen(port);

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

void	NetLoop::sendRouter(const char* data, int len) {
	if( m_pRouterLink != NULL ) {
		m_pRouterLink->send(data, len);
	} else {
		//[TBD]
		//message lost? should bring down this proxy.
		LOG(TAG_DISPATCHER, "send dispatcher failed, m_pRouter==NULL.");		
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

void	NetLoop::timer_cb(int fd, short event, void* arg ) {
	//LOG("NetLoopUdp::timer_cb.");

	NetLoop* loop = (NetLoop*)arg;	
	loop->m_pMgr->onTimer();
}
