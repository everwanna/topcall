#include "netloop.h"
#include "ltmgr.h"
#include "msghandler.h"
#include "linkmgr.h"
#include "link.h"
#include "netlink.h"
#include "netlistener.h"

NetLoop::NetLoop(LTMgr* mgr, const std::string& master_ip, short master_port, short port) 
	: m_pMgr(mgr)
	, m_pMasterLink(NULL)
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		LOG(TAG_LTABLE, "ignore SIGHUP failed.");
	}
#endif
	m_event_base = event_base_new();

	//connect to master link:
	if( master_ip.length()> 0 && master_port != 0 ) {
		m_pMasterLink = new NetLink(this, "master_link", master_ip.c_str(), master_port);
		m_pMasterLink->connect();
	}

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

void	NetLoop::sendMaster(const char* data, int len) {
	if( m_pMasterLink != NULL ) {
		m_pMasterLink->send(data, len);
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
