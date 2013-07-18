#ifndef CONFIG_H
#define CONFIG_H

#ifdef WIN32
#define MEAN_AND_LEAN
#include <WinSock2.h>
#else
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <core/packet.h>
#include <core/res.h>
#include <core/log.h>
#include <core/svid.h>
#include <core/port.h>

#include <protocol/pmproxy.h>
#include <protocol/pmgroup.h>
extern "C" {
	#include <event2/event.h>
	#include <event2/buffer.h>
	#include <event2/bufferevent.h>
}

#define TAG_MPROXY				"mproxy - "
#define NET_BUFFER_SIZE			1024*128
#define MSG_SIZE				1024
#define	MSG_POOL_SIZE			5000*100
#define MSG_CACHE_SIZE			100

#define INVALID_UID				0xffffffff
#define STREAM_EXPIRE_TIME		10		//10 seconds expire with no msg.
#define STREAM_MAX_PKG			100
#define STREAM_CHECK_INTERVAL	3
#define DUMP_INTERVAL			30
#define LOAD_MAX				3000


//feature defines:
#define BLACKLIST_ENABLE		0
#define BLACKLIST_WRONG_PACKET_NUM	100
#define BLACKLIST_PERIOD		60


#define REPORT_LOAD				1
#define REPORT_INTERVAL			30

#define MGROUP_RECONN_INTERVAL	5

#endif
