#ifndef _MONGOLINK_H_
#define _MONGOLINK_H_

#include "config.h"
#include <string>

struct UInfo;
class MongoLink {
public:
	MongoLink(const std::string& dbname);
	~MongoLink();

public:
	int		connect(const std::string& ip, short port);
	UInfo*	query(const std::string& passport);

private:
	mongo	m_mongo;
	std::string	m_strDBName;
};


#endif
