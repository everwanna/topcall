#ifndef CONFIG_H
#define CONFIG_H

#ifdef WIN32
#define MEAN_AND_LEAN
#include <WinSock2.h>
#pragma warning(disable:4996)
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
#include <core/isp.h>

#include <protocol/plogin.h>
#include <protocol/ppush.h>
#include <protocol/pdeamon.h>

extern "C" {
	#include <event2/event.h>
	#include <event2/bufferevent.h>
	#include <event2/buffer.h>
	#include <event2/listener.h>
#include <mongo.h>
}

#define TAG_ROUTER		"dispatcher - "
#define NET_BUFFER_SIZE		1024*128
#define TIMER_INTERVAL		1
#define LINK_CHECK_INTERVAL	5
#define LINK_EXPIRE			15
#define RECONN_INTERVAL		5

#endif

