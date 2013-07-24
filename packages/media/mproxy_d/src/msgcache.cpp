#include "msgcache.h"

MsgCache::MsgCache(int max) : m_nMax(max) {
}

MsgCache::~MsgCache() {
	//[TBD] clean up.
	for( std::map<int, Msg*>::iterator it = m_mapMsgs.begin(); it != m_mapMsgs.end(); it++ ) {
		if( it->second ) {
			delete it->second;
		}
	}
	m_mapMsgs.clear();
}

void	MsgCache::add(int seq, Msg* msg) {	
	if( m_mapMsgs.find(seq) != m_mapMsgs.end() ) {
		//do nothing.
		return;
	}

	if( m_mapMsgs.size() >= (size_t)m_nMax ) {
		int min_seq = m_mapMsgs.begin()->first;
		m_mapMsgs.erase(min_seq);
	} else {
		m_mapMsgs[seq] = msg;
	}
}

Msg*	MsgCache::find(int seq) {
	std::map<int, Msg*>::iterator it = m_mapMsgs.find(seq);
	if( it == m_mapMsgs.end() )
		return NULL;

	return it->second;
}
