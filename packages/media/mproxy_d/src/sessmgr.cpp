#include "sessmgr.h"
#include "session.h"

SessMgr::SessMgr(MPMgr* mgr) 
	: m_pMPMgr(mgr)
	, m_nLastSessId(0)
{
}

Session*	SessMgr::getSession(const std::string& stream) {
	std::map<std::string, int>::iterator it =  m_mapSessIds.find(stream);
	if( it == m_mapSessIds.end() )
		return NULL;

	return getSession(it->second);
}

Session*	SessMgr::getSession(int sid) {
	Session* session = NULL;
	std::map<int, Session*>::iterator it = m_mapSessions.find(sid);
	if( it == m_mapSessions.end() )
		return NULL;

	return it->second;
}

Session*	SessMgr::createSession(const std::string& stream) {
	int sid = 0;

	std::map<std::string, int>::iterator it = m_mapSessIds.find(stream);
	if( it != m_mapSessIds.end() ) {
		LOG(TAG_MPROXY, "SessMgr::createSession, session with name %s exist.", stream.c_str());		
		deleteSession(it->second);
	}

	sid = m_nLastSessId++;
	m_mapSessIds[stream] = sid;

	Session* sess = new Session(this, sid);
	m_mapSessions[sid] = sess;

	return sess;
}

void	SessMgr::deleteSession(int sid) {
	std::map<int, Session*>::iterator it = m_mapSessions.find(sid);
	if( it != m_mapSessions.end() ) {		
		return ;
	}

	Session* sess = it->second;
	m_mapSessions.erase(it);
	delete sess;
}