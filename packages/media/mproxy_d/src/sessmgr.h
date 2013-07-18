#ifndef _SESSMGR_H_
#define _SESSMGR_H_

#include "config.h"
#include <map>

class Session;
class MPMgr;
class SessMgr {
public:
	SessMgr(MPMgr* mgr);

public:
	Session*	getSession(const std::string& stream);
	Session*	getSession(int sid);
	Session*	createSession(const std::string& stream);
	void		deleteSession(int sid);

public:
	inline MPMgr*	getMPMgr() { return m_pMPMgr; }

private:
	MPMgr*		m_pMPMgr;
	int			m_nLastSessId;
	std::map<std::string, int>	m_mapSessIds;
	std::map<int, Session*>	m_mapSessions;	
};

#endif