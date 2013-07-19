#include "mongolink.h"
#include "uinfo.h"

MongoLink::MongoLink() {
	mongo_init(&m_mongo);
}

MongoLink::~MongoLink() {
}

int		MongoLink::connect(const std::string& ip, short port) {	
	LOG(TAG_LOGIN, "mongo connect ip=%s, port=%d.", ip.c_str(), port);

	int status = mongo_client( &m_mongo, ip.c_str(), port );
	if( status != MONGO_OK ) {
		LOG(TAG_LOGIN, "mongo connect failed with code=%d", status);
	} else {
		LOG(TAG_LOGIN, "mongo connected.");		
	}

	return status;
}

UInfo*	MongoLink::query(const std::string& passport) {
	UInfo* uinfo = NULL;
	bson query[1];
	mongo_cursor cursor[1];

	bson_init( query );
	bson_append_string( query, "passport", passport.c_str() );
	bson_finish( query );

	mongo_cursor_init( cursor, &m_mongo, "login.query" );
	mongo_cursor_set_query( cursor, query );

	while( mongo_cursor_next( cursor ) == MONGO_OK ) {
		bson_iterator iterator[1];
		uinfo = new UInfo();

		if ( bson_find( iterator, mongo_cursor_bson( cursor ), "uid" )) {
			uinfo->uid= bson_iterator_int( iterator );			
		} else if ( bson_find( iterator, mongo_cursor_bson( cursor ), "passport" )) {
			uinfo->passport= bson_iterator_string( iterator );
		} else if ( bson_find( iterator, mongo_cursor_bson( cursor ), "password" )) {
			uinfo->password= bson_iterator_string( iterator );
		}		

		goto exit;
	}

exit:
	bson_destroy( query );
	mongo_cursor_destroy( cursor );

	return uinfo;
}





