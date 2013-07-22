#include "netlink.h"
#include "netloop.h"
#include "routemgr.h"
#include "msghandler.h"
#include "routeconfig.h"

NetLink::NetLink(NetLoop* loop, const std::string& name, const std::string& ip, short port) 
	: m_pLooper(loop)
	, m_strName(name)
	, m_strIp(ip)
	, m_nPort(port)
	, m_pEvent(NULL)
	, m_pReconnTimer(NULL)
{
}

void	NetLink::connect() {
	LOG(TAG_ROUTER, "connect %s, ip/port=%s, %d", m_strName.c_str(), m_strIp.c_str(), m_nPort);

	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(m_nPort);
	m_addr.sin_addr.s_addr = inet_addr(m_strIp.c_str());
	m_pEvent = bufferevent_socket_new( m_pLooper->getEventBase(), -1, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(m_pEvent, read_cb, write_cb, event_cb, this);
	bufferevent_enable(m_pEvent, EV_READ|EV_WRITE);

	bufferevent_socket_connect(m_pEvent, (sockaddr*)&m_addr, sizeof(m_addr));
	LOG(TAG_ROUTER, "connect %s, linkid=%d", m_strName.c_str(), bufferevent_getfd(m_pEvent) );
}

void	NetLink::send(const char* msg, int len) 
{
	if( m_pEvent != NULL ) {
		bufferevent_write(m_pEvent, msg, len);
	}
}

void	NetLink::close() {
	if( m_pEvent != NULL ) {		
		bufferevent_free(m_pEvent);
		m_pEvent = NULL;
	}
}

void	NetLink::startReconn() {
	LOG(TAG_ROUTER, "start reconnect %s, ip=%s, port=%d.", m_strName.c_str(), m_strIp.c_str(), m_nPort);
	timeval tm;
	tm.tv_sec = RECONN_INTERVAL;
	tm.tv_usec = 0;
	m_pReconnTimer = event_new( m_pLooper->getEventBase(), -1, EV_READ, reconn_cb, this);
	event_add(m_pReconnTimer, &tm);
}

void	NetLink::stopReconn() {
	//LOG(TAG_MGROUP, "stop reconnect %s.", m_strName.c_str() );
	if( m_pReconnTimer ) {
		event_del(m_pReconnTimer);
		event_free(m_pReconnTimer);
		m_pReconnTimer = NULL;
	}
}

void	NetLink::read_cb(struct bufferevent *bev, void *ctx)
{
	static int seq = 0;
	//LOG(TAG_MGROUP, "NetLoop::read_cb, seq=%d", seq++);	
	NetLink* link = (NetLink*)ctx;
	struct evbuffer *src;
	size_t len;
	char lenbuf[4];
	size_t msg_len;

	src = bufferevent_get_input(bev);	
	while(true) {
		len = evbuffer_get_length(src);	
		if( len <= 4 ) {
			LOG(TAG_ROUTER, "NetLink::read_cb, len<=4");	
			break;
		}

		evbuffer_copyout(src, &lenbuf, 4);
		msg_len = lenbuf[0]<<24|lenbuf[1]<<16 | lenbuf[2]>>8 | lenbuf[3];
		if( msg_len <= len ) {
			evbuffer_remove(src, link->m_pLooper->getBuffer(), msg_len);

			link->m_pLooper->getMgr()->getHandler()->handle(bufferevent_getfd(bev), link->m_pLooper->getBuffer(), msg_len);						
			if( msg_len == len ) {
				break;
			}
		} else {
			//not enough data again, return.
			LOG(TAG_ROUTER, "NetLoop::read_cb, not enough data, waiting. msg_len=%d, len=%d", msg_len, len);	
			break;
		}
	}
}

void	NetLink::write_cb(struct bufferevent *, void *)
{
	//LOG(TAG_MGROUP, "NetLoop::write_cb");	
}

void	NetLink::event_cb(bufferevent *bev, short events, void *user_data) 
{
	NetLink* link = (NetLink*)user_data;
	int linkid = bufferevent_getfd(bev);

	if (events & BEV_EVENT_ERROR || events & BEV_EVENT_EOF ) {
		LOG(TAG_ROUTER, "%s disconnected, linkid=%d", link->m_strName.c_str(), linkid);
		link->close();
		link->stopReconn();
		link->startReconn();
	} else if( events&BEV_EVENT_CONNECTED ) {
		LOG(TAG_ROUTER, "%s connected, linkid=%d, ip=%s, port=%d", link->m_strName.c_str(), linkid, link->m_strIp.c_str(), link->m_nPort);
		link->stopReconn();

		//register the router:
		login::PRegRouterReq req;
		req.name = link->m_pLooper->getMgr()->getConfig()->name;
		
		Pack pk(SVID_LOGIN, login::PRegRouterReq::uri);
		req.marshall(pk);
		pk.pack();

		link->send(pk.getBuf(), pk.getLen());
	}
}

void	NetLink::reconn_cb(int fd, short event, void* arg) {
	NetLink* link = (NetLink*)arg;
	link->connect();
}

