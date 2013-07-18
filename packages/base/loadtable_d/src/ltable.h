#ifndef _LTABLE_H_
#define _LTABLE_H_

#include "config.h"
#include <vector>
#include <map>

struct LTItem;
class LTable {
public:
	LTable();
	~LTable();

public:
	void	addRow(const std::string& key, const std::string& ip, short port, int load, int load_max, const std::string& ext);
	void	removeRow(const std::string& key);
	LTItem*	query();

private:
	int		calcScore(LTItem* item);
	int		calcScore(int load, int load_max);
	void	pushItem(LTItem* item);
	void	popItem(LTItem* item);
	LTItem*	queryInList(std::map<std::string, LTItem*>& list);

private:
	std::map<std::string, LTItem*>	m_mapItems;
	std::map<std::string, LTItem*>	m_mapBestItems;
	std::map<std::string, LTItem*>	m_mapGoodItems;
	std::map<std::string, LTItem*>	m_mapOkItems;
	std::map<std::string, LTItem*>	m_mapBadItems;
};

#endif

