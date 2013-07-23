#include "uidsync.h"
#include "loginmgr.h"
#include "netloop.h"

UidSync::UidSync(LoginMgr* mgr) 
	: m_pLoginMgr(mgr) 
{
}

void	UidSync::onAdd(int uid) {
	m_arrAdds.push_back(uid);
}

void	UidSync::onRemvoe(int uid) {
	m_arrRemoves.push_back(uid);
}

void	UidSync::sync() {
	login::PSyncUidsReq req;
	req.adds = m_arrAdds;
	req.removes = m_arrRemoves;

	Pack pk(SVID_LOGIN, login::PSyncUidsReq::uri);
	req.marshall(pk);
	pk.pack();

	m_pLoginMgr->getLooper()->sendDispatcher( pk.getBuf(), pk.getLen() );
}
