#ifndef _LINK_LAYER_PROXY_H_
#define _LINK_LAYER_PROXY_H_
#include "ilinklayer.h"
#define ARRY_LEN    256

class LinkLayerProxy
    : public ILinkLayer
{
public:
    LinkLayerProxy();
    virtual ~LinkLayerProxy();

public:
    virtual int init(NetMod::Extension* e);
    virtual int connect(uint32_t ip, uint16_t port, int sockType);
    virtual int onConnected();
    virtual int onData(inputbuf_t& input, size_t nrecv);

private:
    enum {
        STATUS_NEW,		// TCP connection est
        STATUS_INIT,	// SOCK5: version/method sent;
        STATUS_AUTH,	// SOCK5: username/pwd sent
        STATUS_CONN,	// SOCK5: auth success, conn request sent  http: CONNECT sent
        STATUS_EST		// SOCk5: connection ested. HTTP: connected
    };
    int         m_proxyType;
    int         m_status;
    char        m_userName[ARRY_LEN];
    char        m_passWd[ARRY_LEN];
    uint32_t    m_uHostIp;
	uint32_t	m_uProxyIp;
    uint16_t    m_uHostPort;
	uint16_t	m_uProxyPort;
};

#endif

