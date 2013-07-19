#ifndef _MONGOLINK_H_
#define _MONGOLINK_H_

#include "config.h"
#include <string>

class MongoLink {
public:
	MongoLink();
	~MongoLink();

public:
	int		connect(const std::string& ip, short port);
	int		verify(int uid, const std::string& passport, const std::string& passwd);

private:
	mongo	m_mongo;
};


#endif
