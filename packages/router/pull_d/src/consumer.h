#ifndef CONSUMER_H
#define CONSUMER_H

#include "config.h"
#include <set>

class Consumer {
public:
	Consumer(int linkid, const std::string& name) : m_nLinkid(linkid), m_strName(name) {
	}

public:	
	void	addTopic(const std::string& topic) {
		m_setTopics.insert(topic);
	}

public:
	int		getLinkId() { return m_nLinkid; }
	std::string getName() { return m_strName; }

private:
	int				m_nLinkid;
	std::string		m_strName;
	std::set<std::string>	m_setTopics;
};


#endif