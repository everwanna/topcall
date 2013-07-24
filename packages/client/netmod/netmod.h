#ifndef _PROTO_NET_MODULE_H_
#define _PROTO_NET_MODULE_H_

#ifdef _WIN32
#define NETMOD_API __declspec(dllexport)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#ifndef uint64_t
typedef unsigned __int64 uint64_t;
#endif
#else // unix or linux
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#ifndef SOCKET
#define SOCKET  int
#endif
#define INVALID_SOCKET (-1)
#define NETMOD_API
#endif

//for test, print sth
#include <iostream>
#include "extension.h"

namespace NetMod
{
#define CONNID_INVALID			(~0)



#define FD_OP_READ  (0x0001)
#define FD_OP_WRITE (0x0010)
#define FD_OP_EXP   (0x0100) //not used currently
#define FD_OP_CLR   (0x1000) //can't used with other opt like FD_SO_READ, or it will clear anyway

enum CONNSTATUS
{
    CONN_INIT = 0,
    CONN_CONNECTING = 1,
    CONN_CONNECTED = 2,
    CONN_CLOSE = 3
};

static volatile long g_curConnId = 0;
#ifdef _WIN32
#define ATOMIC_ADD(p) InterlockedIncrement(p)
#define ATOMIC_GET(p) InterlockedExchangeAdd(p,0)
#else
#define ATOMIC_ADD(p) __sync_add_and_fetch(p, 1);
#endif
/*
* start NetMod, create a thread to run select engine
*/
extern "C" NETMOD_API int NetModStart();

/*
* stop NetMod, remove all connections
*/
extern "C" NETMOD_API int NetModStop();

/*
* element of memory pool
*/
struct NETMOD_API Packet
{
    enum MemType
    {
        MEM_POOL_MAX_T = 0,
        MEM_POOL_MID_T = 1,
        MEM_POOL_MIN_T = 2,
        MEM_NEW_T = 3
    };
    uint32_t	_timestamp;	
    size_t      _bufLen; //length of malloc buffer
    size_t		_dataLen; //length of data
    char*		_data;
    MemType     _type;

    Packet() : _data(NULL), _bufLen(0), _dataLen(0), _type(MEM_NEW_T), _timestamp(0){}
    Packet(char* data, size_t bufLen) : _data(data), _bufLen(bufLen), _dataLen(0), _type(MEM_NEW_T), _timestamp(0){} //pre malloc buffer but not insert data
    ~Packet(){ if(_data) delete _data; _data = NULL; _dataLen = _bufLen = 0; }

    inline void reset(){ memset(_data, 0, _dataLen); _dataLen = 0; }
};

NETMOD_API Packet* PacketAlloc(const char* data, size_t len);
NETMOD_API void PacketRelease(Packet* pkt);

/*
* for compatible, CNetEvent defined like NetIo did.
*/
struct NETMOD_API CNetEvent 
{
    enum 
    {					// Event types
        EV_CONNECTED = 0,
        EV_IN = 1,
        EV_ERROR = 2,
        EV_ALIVE = 3,
        EV_SENT = 4,
        EV_NETWORKLOST = 5,
        EV_NETWORKRESUME = 6,
        EV_CONNSTATE = 7
    };
    int			ConnId;		// actually the value is socket id(SOCKET)
    int			EvtType;	// Event Type
    uint64_t	RetVal;		// more detailed return value
};

struct NETMOD_API CNetEventConnState : public CNetEvent
{
    enum connstate
    {
        CS_TRANSPTLAYER_CONNECTING = 0,
        CS_TRANSPTLAYER_CONNECTED = 1,
        CS_PROXYLAYER_CONNECTING = 2,
        CS_PROXYLAYER_CONNECTED = 3,
        CS_ENCLAYER_START = 4,
        CS_ENCLAYER_FINISH = 5, 
        CS_CONNECTED = 10,
    };

    int state;
    uint32_t timestamp; 
};



class NETMOD_API IEventHandler
{
public:
	virtual ~IEventHandler() {};

	/**
	 * Notification event handlers
	 * 
	 * when a event happens on a connection, the OnEvnet callback will be invoked
	 * @param	evt, specify what's happening on which connection
	 * @param	pkt, pointer to the Packet to be released
	 * @return	0, on success
	 *			ENIO_BUSY, on you are too busy to handle more events
	 * NOTE:	1). the OnEvent() implementation should be very lightweight, NEVER call
	 *			any blocking operation in OnEvent(), or, it MAY block all network
	 *			processing
	 *			2). Do NOT FORGET call PacketRelease(pkt) in some place.
	 */
	virtual int	OnEvent(CNetEvent* evt, Packet* pkt) = 0;
};

struct NETMOD_API ConnAttr{
    const static int MAX_EXTENSIONS = 16;
    enum 
    {
        CONN_UDP = SOCK_DGRAM,	// a UDP connection
        CONN_TCP = SOCK_STREAM,	// a TCP connection
    };
    /// CONN_UDP or CONN_TCP
    int		ConnType;
    /// The local address, if the ConnType is UDP, LocalAddr must be specified
    int		LocalIP;
    short	LocalPort;
    /// The remote address, if the ConnType is TCP, RemoteAddr must be specified
    int		RemoteIP;
    short	RemotePort;
    /// Event Handler, the reference to the EventHandler, on event happens, the
    /// evHandler->OnEvent() will be invorked
    IEventHandler*		evHandler;
    /// The extension layers want to be added to the connection, end with a NULL
    /// Set the exts in order, if you want a multiport, compressed, encrypted conn using a 
    /// proxy, exts[0] = ExtMultiport, exts[1] = ExtCompress, exts[2] = ExtEncryption, 
    /// exts[3] = ExtProxy, exts[4] = NULL; 
    Extension*			exts[MAX_EXTENSIONS];
};

/*
* create a async connection that unconnected yet, need to call ConnConnect to do connect
* @param attr:      use this struct to specify what kinds of connection you want to create
* @return int:      connection ID, INVALID_SOCKET(-1) will be returned if failed
*/
NETMOD_API int ConnCreate(ConnAttr* attr);

/*
* do connect
* @param connid : the connection id that ConnCreate returned, it's a connId to get associated connection in ConnMgr
* @param ip     : target server ip
* @param port   : target server port
* @return       : int, return 0 if success, else -1 will be given
*/
NETMOD_API int ConnConnect(int connid, uint32_t ip = 0, uint16_t port = 0);

/*
* send data
* @param connid : the socket id that ConnCreate returned, it's a connId to get associated connection in ConnMgr
* @param pkt    : wrapper of sent data
*/
NETMOD_API int ConnSend(int connid, Packet* pkt);

/*
* close a connection
* @param connid : the connection ID id that ConnCreate returned, it's a connId to get associated connection in ConnMgr
*/
NETMOD_API int ConnClose(int connid);

/*
* 
* @param	evHandler,	the evHandler will be unregister
* @return	int,		the number of connection will be closed silently
*/
//NETMOD_API int UnregEvHandler(IEventHandler* ev);
}

#endif


