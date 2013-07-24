#include "pushmgr.h"
#include "store.h"
#include "netloop.h"
#include "msghandler.h"
#include "consumer.h"


PushMgr::PushMgr(PushConfig& config) 
	: m_config(config)
{
	m_pLooper = new NetLoop(this, m_config.port);
	m_pHandler = new MsgHandler(this);
	m_pStore = new Store(this);
}

PushMgr::~PushMgr() {
	if( m_pLooper ) {
		delete m_pLooper;
	}
	if( m_pHandler ) {
		delete m_pHandler;
	}
	if( m_pStore ) {
		delete m_pStore;
	}
}

int		PushMgr::registerConsumer(int linkid, const std::string& name, const std::string& topic) {
	Consumer* old = NULL;
	Consumer* consumer = NULL;

	if( !hasTopic(topic) ) {
		LOG(TAG_PUSH, "PushMgr.registerConsumer, topic doesn't exist, create topic %d", topic);
		createTopic(topic);
		
		if( !hasTopic(topic) ) {
			LOG(TAG_PUSH, "PushMgr.registerConsumer, topic doesn't exist, topic=%d, FAIL!!!@", topic);			
			return RES_FAIL;
		}
	}

	if( m_mapConsumers.find(name) != m_mapConsumers.end() ) {
		old = m_mapConsumers[name];		
		if( old->getLinkId() == linkid ) {
			old->addTopic(topic);
			m_pStore->subscribe(old, topic);
		} else {
			//linkid is different, but with the same consumer name, log and fail out:
			LOG(TAG_PUSH, "PushMgr.registerConsumer, consumer with the same name already exist, name=%s", name.c_str());
			return RES_FAIL;
		}
	} else {
		//consumer doesn't exist yet, create it:
		consumer = new Consumer(linkid, name);
		consumer->addTopic(topic);

		m_mapConsumers[name] = consumer;
		m_pStore->subscribe(consumer, topic);
	}
	m_pStore->dump();

	return RES_OK;
}

void	PushMgr::deleteConsumer(int linkid) {
	LOG(TAG_PUSH, "PushMgr.deleteConsumer, linkid=%d", linkid);

	std::map<std::string, Consumer*>::iterator it;
	for( it = m_mapConsumers.begin(); it != m_mapConsumers.end(); it++ ) {
		if( it->second->getLinkId() == linkid ) {
			Consumer* consumer = it->second;
			m_pStore->unsubscribe(consumer->getName());
			m_mapConsumers.erase(it);
			delete consumer;
			break;
		}
	}

	m_pStore->dump();
}

void	PushMgr::createTopic(const std::string& topic) {
	m_pStore->createTopic(topic);
}

void	PushMgr::deleteTopic(const std::string& topic) {
	m_pStore->deleteTopic(topic);
}

bool	PushMgr::hasTopic(const std::string& topic) {
	return m_pStore->hasTopic(topic);
}

void	PushMgr::push(const std::string& topic, const std::string& payload) {
	m_pStore->push(topic, payload);
}

void	PushMgr::run() {
	if( m_pLooper ) {
		m_pLooper->run();
	}
}

void	PushMgr::onTimer() {
}
