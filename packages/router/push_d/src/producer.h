#ifndef PRODUCER_H
#define PRODUCER_H

#include <vector>

class Producer {
public:
	Producer(int linkid, const std::string& name) : m_nLinkId(linkid), m_strName(name)
	{
	}

public:
	void	push(char* msg, int len);

public:
	int		getLinkId() { return m_nLinkId; }

private:
	int		m_nLinkId;
	std::string m_strName;
};


#endif