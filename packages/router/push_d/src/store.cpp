#include "store.h"
#include "pushmgr.h"
#include "netloop.h"
#include "consumer.h"

Store::Store(PushMgr* mgr) {
	m_pPushMgr = mgr;
}

Store::~Store() {
}

void	Store::createTopic(const std::string& topic) {
	m_setTopics.insert(topic);

	std::list<Consumer*> consumers;
	m_mapTopics[topic] = consumers;
}

void	Store::deleteTopic(const std::string& topic) {
	if( m_setTopics.find(topic) == m_setTopics.end() ) 
		return;

	m_setTopics.erase(topic);

	//need to update m_mapTopics, and send each consumer a msg:
}

bool	Store::hasTopic(const std::string& topic) {
	return m_setTopics.find(topic) != m_setTopics.end();
}

void	Store::subscribe(Consumer* consumer, const std::string& topic) {
	if( !hasTopic(topic) ) {
		LOG(TAG_PUSH, "Store::subscribe, topic %d doesn't exist", topic);
		return;
	}

	for( std::list<Consumer*>::iterator it = m_mapTopics[topic].begin(); it != m_mapTopics[topic].end(); it++ ) {
		if( (*it)->getLinkId() == consumer->getLinkId() ) {
			LOG(TAG_PUSH, "Store::subscribe, consumer %s already subscribe to topic %d", consumer->getName().c_str(), topic);
			return;
		}
	}
	m_mapTopics[topic].push_back(consumer);
}

void	Store::unsubscribe(const std::string& name) {
	std::map<std::string, std::list<Consumer*> >::iterator it1;

	for( it1 = m_mapTopics.begin(); it1 != m_mapTopics.end(); it1++ ) {
		std::string topic = it1->first;

		for( std::list<Consumer*>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++ ) {
			if( (*it2)->getName() == name ) {
				LOG(TAG_PUSH, "Store::unsubscribe, remove consumer %s from topic ", name.c_str(), topic);
				it1->second.erase( it2 );
				return;
			}
		}
	}
}

void	Store::unsubscribe(const std::string& name, const std::string& topic) {
	if( !hasTopic(topic) ) {
		LOG(TAG_PUSH, "Store::subscribe, topic %d doesn't exist", topic);
		return;
	}

	for( std::list<Consumer*>::iterator it = m_mapTopics[topic].begin(); it != m_mapTopics[topic].end(); it++ ) {
		if( (*it)->getName() == name ) {
			LOG(TAG_PUSH, "Store::unsubscribe, remove consumer %s from topic ", name.c_str(), topic);
			m_mapTopics[topic].erase( it );
			return;
		}
	}

	LOG(TAG_PUSH, "Store::subscribe, consumer %s for topic %d doesn't exist", name.c_str(), topic);
}

void	Store::push(const std::string& topic, const std::string& payload) {
	if( !hasTopic(topic) ) {
		LOG(TAG_PUSH, "Store::push, topic %s not found.", topic);
		return;
	}

	Consumer* consumer = NULL;
	for( std::list<Consumer*>::iterator it = m_mapTopics[topic].begin(); it != m_mapTopics[topic].end(); it++ ) {
		consumer = (*it);		
		m_pPushMgr->getLooper()->send(consumer->getLinkId(), payload.c_str(), payload.length());
	}
}

void	Store::dump() {
	
}