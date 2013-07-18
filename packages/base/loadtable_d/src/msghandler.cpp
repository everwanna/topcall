#include "msghandler.h"
#include "ltmgr.h"
#include "netloop.h"
#include "ltable.h"
#include "ltitem.h"
#include "linkmgr.h"
#include "slavemgr.h"

MsgHandler::MsgHandler(LTMgr* mgr) 
	: m_pLTMgr(mgr)
{
	m_pBuffer = new char[1024];
}

MsgHandler::~MsgHandler() {
	if( m_pBuffer ) 
		delete m_pBuffer;
}

void	MsgHandler::handle(int linkid, char* msg, int len) {
#ifdef _DEBUG
	//LOG(TAG_MCENTER, "MsgHandler::handle, linkid=%d, len=%d", linkid, len);
#endif
	Unpack up(msg, len);
	up.popHead();

	switch(up.getUri() ) {
	case URI_LOAD_REPORT:
		onLTReport(linkid, &up);
		break;
	case URI_LOAD_QUERY_REQ:
		onLTQueryReq(linkid, &up);
		break;	
	case URI_LOAD_QUERYJSON_REQ:
		onLTQueryJsonReq(linkid, &up);
		break;	
	case URI_LOAD_REGSLAVE_REQ:
		onLTRegSlaveReq(linkid, &up);
		break;
	case URI_LOAD_PING:
		onLTPing(linkid, &up);
		break;
	}
}

void	MsgHandler::onLTReport(int linkid, Unpack* up) {
	PLTReport req;
	req.unmarshall(*up);

	LOG(TAG_LTABLE, "MsgHandler::onLTReport, key/ip/port/load/load_nax=%s, %s, %d, %d, %d", req.key.c_str(), req.ip.c_str(), req.port, 
		req.load, req.load_max);

	m_pLTMgr->getLTable()->addRow(req.key.c_str(), req.ip.c_str(), req.port, req.load, req.load_max, req.ext);

	//yeah, copy the same thing to all slaves:
	m_pLTMgr->getSlaveMgr()->send(up->getBuf(), up->getLen());
}

void	MsgHandler::onLTQueryReq(int linkid, Unpack* up) {
	PLTQueryReq req;
	req.unmarshall(*up);

	LOG(TAG_LTABLE, "MsgHandler::onLTReport");

	LTItem* item = m_pLTMgr->getLTable()->query();
	if( item == NULL ) {
		m_queryRes.res = RES_FAIL;
	} else {
		m_queryRes.res = RES_OK;
	}
}

void	MsgHandler::onLTQueryJsonReq(int linkid, Unpack* up) {	
	PLTQueryJsonRes res;

	PLTQueryJsonReq req;
	req.unmarshall(*up);
	LOG(TAG_LTABLE, "MsgHandler::onLTQueryJsonReq");

	memset(m_pBuffer, 0, 1024);
	LTItem* item = m_pLTMgr->getLTable()->query();
	if( item == NULL ) {	
		sprintf(m_pBuffer, "{res:%d}", RES_FAIL);
	} else {		
		sprintf(m_pBuffer, "{res:%d, ip:%s, port:%d, load:%d, load_max:%d}", RES_OK, item->ip.c_str(), item->port, item->load, item->load_max);		
	}
	res.json.assign(m_pBuffer);

	//[TBD] help static this.
	Pack pk(SVID_LOADTABLE, PLTQueryJsonRes::uri);
	res.marshall(pk);
	pk.pack();

	m_pLTMgr->getLinkMgr()->send(linkid, pk.getBuf(), pk.getLen());
}

void	MsgHandler::onLTRegSlaveReq(int linkid, Unpack* up) {
	PLTRegSlaveReq req;
	req.unmarshall(*up);

	LOG(TAG_LTABLE, "register slave, linkid/name=%d, %s", linkid, req.name.c_str());
	m_pLTMgr->getSlaveMgr()->add(linkid, req.name, m_pLTMgr->getLooper()->getSystemTime());
}


void	MsgHandler::onLTPing(int linkid, Unpack* up) {
	m_pLTMgr->getSlaveMgr()->update(linkid, m_pLTMgr->getLooper()->getSystemTime());
}

