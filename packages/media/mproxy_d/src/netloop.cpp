#include "netloop.h"
#include "mpmgr.h"
#include "msghandler.h"

NetLoop::NetLoop(MPMgr* mgr, const std::string& mg_ip, int mg_port, int port) 
	: m_pMPMgr(mgr)
	, m_pMGLink(NULL)
	, m_strMGIp(mg_ip)
	, m_nMGPort(mg_port)
	, m_pMGReconnTimer(NULL)
{
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		LOG(TAG_MPROXY, "ignore SIGHUP failed.");
	}
#endif
	m_event_base = event_base_new();

	{
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		int yes = 1;
		setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int));

		sin.sin_port = htons(port);
		bind(s, (const sockaddr*)&sin, sizeof(sin));
		event* ev = event_new(m_event_base, s, EV_READ|EV_PERSIST, event_cb, this);
		event_add(ev, NULL);
	}
	m_pBuffer = new char[NET_BUFFER_SIZE];

	//connect to MGroup
	connectMG();

	//start a timer to do scheduler job:
	{
		timeval tm;
		tm.tv_sec = REPORT_INTERVAL;
		tm.tv_usec = 0;
		m_timer = event_new(m_event_base, -1, EV_READ|EV_PERSIST, timer_cb, this);
		evtimer_add(m_timer, &tm);
	}
}

NetLoop::~NetLoop()
{
	if( m_timer ) {
		evtimer_del(m_timer);
	}
	if( m_event_base )
	{
		event_base_free(m_event_base);
		m_event_base = NULL;
	}
}

void	NetLoop::run() 
{
	event_base_dispatch(m_event_base);
}


void	NetLoop::send(int linkid, const sockaddr_in* addr, const char* msg, int len) 
{
	int ret = sendto(linkid, msg, len, 0, (sockaddr*)addr, sizeof(sockaddr_in)); 
	if( ret <= 0 ) {
		LOG(TAG_MPROXY, "NetLoop::send, failed, ret=%d, linkid=%d, ip/port=%d:%d", ret, linkid, addr->sin_addr.s_addr, addr->sin_port);
	}
}

int		NetLoop::getSystemTime() {
	timeval tm;
	evutil_gettimeofday(&tm, NULL);
	return tm.tv_sec;
}

void	NetLoop::connectMG() {
	LOG(TAG_MPROXY, "Connect to mgroup, ip/port=%s, %d", m_strMGIp.c_str(), m_nMGPort);

	memset(&m_addrMG, 0, sizeof(m_addrMG));
	m_addrMG.sin_family = AF_INET;
	m_addrMG.sin_port = htons(m_nMGPort);
	m_addrMG.sin_addr.s_addr = inet_addr(m_strMGIp.c_str());

	m_pMGLink = bufferevent_socket_new(m_event_base, -1, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(m_pMGLink, mgroup_read_cb, mgroup_write_cb, mgroup_event_cb, this);
	bufferevent_socket_connect(m_pMGLink, (sockaddr*)&m_addrMG, sizeof(m_addrMG));
	bufferevent_enable(m_pMGLink, EV_READ);

	LOG(TAG_MPROXY, "Connecting to mgroup, linkid=%d", bufferevent_getfd(m_pMGLink) );
	LOG(TAG_MPROXY, "Connecting..." );
}

void	NetLoop::sendMG(const char* msg, int len) 
{
	if( m_pMGLink != NULL ) {
		bufferevent_write(m_pMGLink, msg, len);
	}
}

void	NetLoop::closeMG() {
	if( m_pMGLink != NULL ) {		
		bufferevent_free(m_pMGLink);
		m_pMGLink = NULL;
	}
}

void	NetLoop::startReconnMG() {
	//bufferevent_socket_connect( m_pMCLink, (sockaddr*)&m_addrMc, sizeof(m_addrMc));
	timeval tm;
	tm.tv_sec = MGROUP_RECONN_INTERVAL;
	tm.tv_usec = 0;
	m_pMGReconnTimer = event_new(m_event_base, -1, EV_READ, mgroup_reconn_cb, this);
	event_add(m_pMGReconnTimer, &tm);
}

void	NetLoop::stopReconnMG() {
	if( m_pMGReconnTimer ) {
		event_del(m_pMGReconnTimer);
		event_free(m_pMGReconnTimer);
		m_pMGReconnTimer = NULL;
	}
}

void	NetLoop::event_cb(int fd, short event, void* arg) 
{
	if( event == EV_READ ) {
		read_cb(fd, event, arg);
	} else {
		LOG(TAG_MPROXY, "NetLoop::event_cb, UNKNOWN event=%d.", event);
	}
}

void	NetLoop::read_cb(int fd, short event, void* arg)
{
#ifdef _DEBUG
	static int seq = 0;
	//LOG("NetLoop::read_cb, seq=%d", seq++);	
#endif

	NetLoop* loop = (NetLoop*)arg;	
	size_t len;
	sockaddr_in from;
	ev_socklen_t from_len = sizeof(from);

	memset(&from, 0, sizeof(from));	
	len = recvfrom(fd, loop->m_pBuffer, NET_BUFFER_SIZE, 0, (sockaddr*)&from, &from_len);
	if( len <= 4 ) {
		LOG(TAG_MPROXY, "NetLoop::read_cb, len <= 4 || msg_len>len-4, len=%d.", len);	
		return;
	}

	loop->m_pMPMgr->getHandler()->handle(fd, &from, loop->m_pBuffer, len);							
}

void	NetLoop::timer_cb(int fd, short event, void* arg ) {
	//LOG("NetLoop::timer_cb.");

	NetLoop* loop = (NetLoop*)arg;	
	loop->m_pMPMgr->schedule();
}


//mgroup:
void	NetLoop::mgroup_read_cb(struct bufferevent *bev, void *ctx)
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
			LOG(TAG_MPROXY, "NetLoop::mgroup_read_cb, len<=4");	
			break;
		}

		//evbuffer_remove(src, &msg_len, 4);
		evbuffer_copyout(src, &lenbuf, 4);
		msg_len = lenbuf[0]<<24|lenbuf[1]<<16 | lenbuf[2]>>8 | lenbuf[3];
		if( msg_len <= len ) {			
			evbuffer_remove(src, loop->m_pBuffer, msg_len);

			loop->m_pMPMgr->getHandler()->handle(bufferevent_getfd(bev), NULL, loop->m_pBuffer, msg_len);						
			if( msg_len == len )
				break;
		} else {
			//not enough data again, return.
			LOG(TAG_MPROXY, "NetLoop::mgroup_read_cb, not enough data, waiting. msg_len=%d, len=%d", msg_len, len);	
			break;
		}
	}
}

void	NetLoop::mgroup_write_cb(struct bufferevent *, void *)
{
	//LOG(TAG_MGROUP, "NetLoop::write_cb");	
}

void	NetLoop::mgroup_event_cb(bufferevent *bev, short events, void *user_data) 
{
	NetLoop* loop = (NetLoop*)user_data;
	int linkid = bufferevent_getfd(bev);

	if (events & BEV_EVENT_ERROR | events & BEV_EVENT_EOF ) {
		LOG(TAG_MPROXY, "Disconnected from mproxy, linkid=%d", linkid);
		loop->closeMG();
		loop->stopReconnMG();
		loop->startReconnMG();
	} else if( events&BEV_EVENT_CONNECTED ) {
		LOG(TAG_MPROXY, "Connected to mproxy, linkid=%d", linkid);
		LOG(TAG_MPROXY, "Waiting for requests...");
		LOG(TAG_MPROXY, "================================================");
		loop->stopReconnMG();
	}
}

void	NetLoop::mgroup_reconn_cb(int fd, short event, void* arg) {
	NetLoop* loop = (NetLoop*)arg;
	loop->connectMG();
}
