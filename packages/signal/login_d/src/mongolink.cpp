#include "mongolink.h"
#include "uinfo.h"

MongoLink::MongoLink(const std::string& dbname) 
	: m_strDBName(dbname)
{
	mongo_init(&m_mongo);
}

MongoLink::~MongoLink() {
	mongo_destroy(&m_mongo);
}

int		MongoLink::connect(const std::string& ip, short port) {	
	LOG(TAG_LOGIN, "mongo connect ip=%s, port=%d.", ip.c_str(), port);

	int status = mongo_client( &m_mongo, ip.c_str(), port );
	if( status != MONGO_OK ) {
		LOG(TAG_LOGIN, "mongo connect failed with code=%d", status);
	} else {
		LOG(TAG_LOGIN, "mongo connected.");		
	}

	//test code here:
	//query("yunzed");

	return status;
}

UInfo*	MongoLink::query(const std::string& passport) {
	UInfo* uinfo = NULL;
	bson query[1];
	mongo_cursor cursor[1];

	bson_init( query );
	bson_append_string( query, "passport", passport.c_str() );
	bson_finish( query );

	mongo_cursor_init( cursor, &m_mongo, m_strDBName.c_str() );
	mongo_cursor_set_query( cursor, query );

	if( mongo_cursor_next( cursor ) == MONGO_OK ) {
		bson_iterator iterator[1];
		uinfo = new UInfo();
		//uinfo->linkid = 0;

		if ( bson_find( iterator, mongo_cursor_bson( cursor ), "uid" ) != BSON_EOO ) {
			uinfo->uid= bson_iterator_int( iterator );			
		} 
		
		if ( bson_find( iterator, mongo_cursor_bson( cursor ), "passport" ) != BSON_EOO ) {
			uinfo->passport= bson_iterator_string( iterator );
		} 
		
		if ( bson_find( iterator, mongo_cursor_bson( cursor ), "password" ) != BSON_EOO ) {
			uinfo->password= bson_iterator_string( iterator );
		}		

		if ( bson_find( iterator, mongo_cursor_bson( cursor ), "router" ) != BSON_EOO ) {
			uinfo->router= bson_iterator_string( iterator );
		}		

		if ( bson_find( iterator, mongo_cursor_bson( cursor ), "dispatcher" ) != BSON_EOO ) {
			uinfo->dispatcher= bson_iterator_string( iterator );
		}	

		if ( bson_find( iterator, mongo_cursor_bson( cursor ), "proxy" ) != BSON_EOO ) {
			uinfo->proxy= bson_iterator_string( iterator );
		}	

	} else {
		LOG(TAG_LOGIN, "query passport failed, passport=%s", passport.c_str());
	}

exit:
	bson_destroy( query );
	mongo_cursor_destroy( cursor );

	return uinfo;
}



