#include "mongolink.h"

MongoLink::MongoLink(const std::string& dbname) 
	: m_strDBName(dbname)
	, m_bConnected(false)
{
	mongo_init(&m_mongo);
}

MongoLink::~MongoLink() {
	mongo_destroy(&m_mongo);
}

int		MongoLink::connect(const std::string& ip, short port) {	
	LOG(TAG_PUSH, "mongo connect ip=%s, port=%d.", ip.c_str(), port);

	//int status = mongo_client( &m_mongo, ip.c_str(), port );
	int status = mongo_client( &m_mongo, "127.0.0.1", 27017 );
	if( status != MONGO_OK ) {
		LOG(TAG_PUSH, "mongo connect failed with code=%d", status);
		m_bConnected = false;
	} else {
		LOG(TAG_PUSH, "mongo connected.");	
		m_bConnected = true;
	}

	//test code here:
	//query("yunzed");

	return status;
}

void	MongoLink::save(const std::string& topic, const std::string& data) {

}


