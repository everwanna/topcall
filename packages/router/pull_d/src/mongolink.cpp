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
	LOG(TAG_PULL, "mongo connect ip=%s, port=%d.", ip.c_str(), port);

	//int status = mongo_client( &m_mongo, ip.c_str(), port );
	int status = mongo_client( &m_mongo, "127.0.0.1", 27017 );
	if( status != MONGO_OK ) {
		LOG(TAG_PULL, "mongo connect failed with code=%d", status);
		m_bConnected = false;
	} else {
		LOG(TAG_PULL, "mongo connected.");	
		m_bConnected = true;
	}

	//test code here:
	//query("yunzed");

	return status;
}

std::string	MongoLink::fetch() {
	if( !m_bConnected )
		return "";

	std::string msg;
	bson query[1];
	mongo_cursor cursor[1];

	bson_init( query );
	bson_finish( query );

	mongo_cursor_init( cursor, &m_mongo, m_strDBName.c_str() );
	mongo_cursor_set_query( cursor, query );
	//mongo_cursor_set_limit( cursor, 1);

	if( mongo_cursor_next( cursor ) == MONGO_OK ) {
		bson_iterator iterator[1];		

		if ( bson_find( iterator, mongo_cursor_bson( cursor ), "data" ) != BSON_EOO ) {	
			msg = bson_iterator_string(iterator);
		} 

	} else {
		LOG(TAG_PULL, "query msg failed, db=%s.", m_strDBName.c_str());
	}

exit:
	bson_destroy( query );
	mongo_cursor_destroy( cursor );

	return msg;
}



