#ifndef STORE_H
#define STORE_H

#include "config.h"
#include <map>
#include <set>
#include <list>
#include <string>

class PullMgr;
class Msg;
class Consumer;
class Store {
public:
	Store(PullMgr* mgr);
	~Store();

public:
	void	createTopic(const std::string& topic);
	void	deleteTopic(const std::string& topic);
	bool	hasTopic(const std::string& topic);
	void	subscribe(Consumer* consumer, const std::string& topic);
	void	unsubscribe(const std::string& name);
	void	unsubscribe(const std::string& name, const std::string& topic);
	void	push(const std::string& topic, const char* data, int len);
	void	dump();

private:
	PullMgr*		m_pPushMgr;
	int				m_nTopic;
	std::set<std::string>	m_setTopics;
	std::map<std::string, std::list<Consumer*> >	m_mapTopics;
};

#endif