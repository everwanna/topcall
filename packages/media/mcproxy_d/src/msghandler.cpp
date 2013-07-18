#include "msghandler.h"
#include "mcpmgr.h"
#include "netloop.h"
#include "cache.h"
#include "pendlist.h"
#include "statistic.h"

MsgHandler::MsgHandler(MCPMgr* mgr) 
	: m_badGrpPack(SVID_MCENTER, PMCAllocRes::uri)
	, m_packPreAllocRes(SVID_MCENTER, PMCPreAllocReq::uri)
	, m_packAllocRes(SVID_MCENTER, PMCAllocRes::uri)
{
	m_pMCPMgr = mgr;

	PMCAllocRes res;
	res.res = RES_FAIL;
	res.ip = "";
	res.port = 0;
	
	res.marshall(m_badGrpPack);
	m_badGrpPack.pack();
}

MsgHandler::~MsgHandler() {
}

void	MsgHandler::handle(int linkid, char* msg, int len) {
#ifdef _DEBUG
	//LOG(TAG_MCPROXY, "MsgHandler::handle, linkid=%d, len=%d", linkid, len);
#endif
	//should be optimized, no stack var.
	Unpack up(msg, len);
	up.popHead();
	
	if( up.getSvid() != SVID_MCENTER ) {
		LOG(TAG_MCPROXY, "MsgHandler::handle, svid!=SVID_MCENTER|SVID_MCPROXY, svid=%d", up.getSvid());
		//[TBD]
		//Should consider the blocklist here.
		return;
	}

	switch(up.getUri() ) {
	case URI_MCENTER_ALLOC_REQ:
		onMCAllocReq(linkid, &up);
		break;
	case URI_MCENTER_PREALLOC_RES:
		onMCPreAllocRes(linkid, &up);
		break;
	}
}

/**
 * onAllocProxyReq
 *
 * The command need to be forward to mgroup. mgroup allocate a proxy and returns to mcenter.
 * mcenter will forward the response back to client.
 */
void	MsgHandler::onMCAllocReq(int linkid, Unpack* up) {
	PMCAllocReq req;
	req.unmarshall(*up);

	LOG(TAG_MCPROXY, "MsgHandler.onMCAllocReq, alloc for isp1/area1/isp2/area2=%d,%d,%d,%d",
		req.isp1, req.area1, req.isp2, req.isp2);

	static char strisp[64];
	sprintf(strisp, "%d:%d:%d:%d", req.isp1, req.area1, req.isp2, req.area2);
	std::string data = m_pMCPMgr->getCache()->get(strisp);
	if( data.length() == 0 ) {
		//m_pMCPMgr->getLooper()->send(linkid, m_badGrpPack.getBuf(), m_badGrpPack.getLen());
		//send the request to mcenter:
		//m_pMCPMgr->getLooper()->send();
		PMCPreAllocReq prereq;
		prereq.isp1 = req.isp1;
		prereq.area1 = req.area1;
		prereq.isp2 = req.isp2;
		prereq.area2 = req.area2;
		prereq.num = CACHE_SIZE;

		//Pack pk(SVID_MCENTER, PMCPreAllocReq::uri);
		//prereq.marshall(pk);
		//pk.pack();		
		m_packPreAllocRes.reset();
		prereq.marshall(m_packPreAllocRes);
		m_packPreAllocRes.pack();
		m_pMCPMgr->getLooper()->sendMC(m_packPreAllocRes.getBuf(), m_packPreAllocRes.getLen());

		//pending this request:
		m_pMCPMgr->getPendList()->add(strisp, linkid);
	} else {
		m_pMCPMgr->getLooper()->send(linkid, data.c_str(), data.length());

		//if we are out of cache, try refetch some:
		if( m_pMCPMgr->getCache()->getSize(strisp) < CACHE_MINIMAL ) {
			PMCPreAllocReq prereq;
			prereq.isp1 = req.isp1;
			prereq.area1 = req.area1;
			prereq.isp2 = req.isp2;
			prereq.area2 = req.area2;
			prereq.num = CACHE_SIZE;

			//Pack pk(SVID_MCENTER, PMCPreAllocReq::uri);
			m_packPreAllocRes.reset();
			prereq.marshall(m_packPreAllocRes);
			m_packPreAllocRes.pack();		
			m_pMCPMgr->getLooper()->sendMC(m_packPreAllocRes.getBuf(), m_packPreAllocRes.getLen());
		}
	}

	//add to statistic:
	m_pMCPMgr->getStatistic()->addIspReq(strisp);
}

void	MsgHandler::onMCPreAllocRes(int linkid, Unpack* up) {
	PMCPreAllocRes res;
	res.unmarshall(*up);

	static char strisp[64];
	memset(strisp, 0, 64);
	sprintf(strisp, "%d:%d:%d:%d", res.isp1, res.area1, res.isp2, res.area2);

	LOG(TAG_MCPROXY, "MsgHandler.onMCPreAllocRes, prealloc for isp/num/res=%s, %d, %d",
		strisp, res.num, res.res);

	if( res.res != RES_OK ) {
		LOG(TAG_MCPROXY, "Msghandler.onMCPreAllocRes, failed, isp/res=%s, %d.", 
			strisp, res.res );

		//fill all pending requests:
		std::set<int> links = m_pMCPMgr->getPendList()->get(strisp);
		if( links.size() != 0 ) {
			for( std::set<int>::iterator it = links.begin(); it != links.end(); it++ ) {
				m_pMCPMgr->getLooper()->send( *it, m_badGrpPack.getBuf(), m_badGrpPack.getLen() );
			}
		}
		m_pMCPMgr->getPendList()->remove(strisp);
	}  else {
		//update the cache:
		PMCAllocRes cacheres;
		cacheres.res = RES_OK;
		cacheres.ip = res.ip;
		cacheres.port = res.port;
		cacheres.isp1 = res.isp1;
		cacheres.area1 = res.area1;	
		cacheres.isp2 = res.isp1;
		cacheres.area2 = res.area1;

		//Pack pk(SVID_MCENTER, PMCAllocRes::uri);
		//cacheres.marshall(pk);
		m_packAllocRes.reset();
		cacheres.marshall(m_packAllocRes);
		m_packAllocRes.pack();	

		m_pMCPMgr->getCache()->add(strisp, m_pMCPMgr->getLooper()->getSystemTime(), res.num, std::string(m_packAllocRes.getBuf(), m_packAllocRes.getLen()));
		//fill all pending requests:
		std::set<int> links = m_pMCPMgr->getPendList()->get(strisp);
		if( links.size() != 0 ) {
			for( std::set<int>::iterator it = links.begin(); it != links.end(); it++ ) {
				m_pMCPMgr->getLooper()->send( *it, m_packAllocRes.getBuf(), m_packAllocRes.getLen() );
			}
		}
		m_pMCPMgr->getPendList()->remove(strisp);
	}	
}
