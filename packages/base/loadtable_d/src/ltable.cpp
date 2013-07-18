#include "ltable.h"
#include "ltitem.h"

#define SCORE_BEST		100
#define SCORE_GOOD		86
#define SCORE_OK		60
#define SCORE_BAD		10

LTable::LTable() {
}

LTable::~LTable() {
	//[TBD] remove all objects.
}

void	LTable::addRow(const std::string& key, const std::string& ip, short port, int load, int load_max, const std::string& ext) {
	std::map<std::string, LTItem*>::iterator it = m_mapItems.find(key);
	if( it == m_mapItems.end() ) {
		//an new item:
		LTItem* item = new LTItem();
		item->key = key;
		item->ip = ip;
		item->port = port;
		item->load = load;
		item->load_max = load_max;
		item->ext = ext;
		item->score = calcScore(item);
		item->reserve = 0;

		m_mapItems[key] = item;		
		pushItem(item);
	} else {
		//calc the new score:
		int score = calcScore(load, load_max);
		if( score == it->second->score ) {
			//the score is still the same
			it->second->load = load;
			it->second->load_max = load_max;
			it->second->reserve = 0;
		} else {
			//the score changed, need to change list:
			popItem(it->second);
			it->second->load = load;
			it->second->load_max = load_max;
			it->second->reserve = 0;
			it->second->score = score;
			pushItem(it->second);
		}
	}
}

void	LTable::removeRow(const std::string& key) {

}

LTItem*	LTable::query() {
	LTItem* item = NULL;

	item = queryInList(m_mapBestItems);
	if( item != NULL )
		return item;

	item = queryInList(m_mapGoodItems);
	if( item != NULL )
		return item;

	item = queryInList(m_mapOkItems );
	if( item != NULL ) 
		return item;

	return NULL;
}

int		LTable::calcScore(LTItem* item) {
	return calcScore(item->load, item->load_max);
}

int		LTable::calcScore(int load, int load_max) {
	if( load_max == 0 ) {
		return 0;
	}

	int percent = load*100/load_max;
	if( percent >= 100 )
		return SCORE_BAD;
	else if( percent >= 75 )
		return SCORE_OK;
	else if( percent >= 50 )
		return SCORE_GOOD;
	else 
		return SCORE_BEST;
}

void	LTable::pushItem(LTItem* item) {
	switch(item->score) {
	case SCORE_BEST:
		m_mapBestItems[item->key] = item;
		break;
	case SCORE_GOOD:
		m_mapGoodItems[item->key] = item;
		break;
	case SCORE_OK:
		m_mapOkItems[item->key] = item;
		break;
	case SCORE_BAD:
		m_mapBadItems[item->key] = item;
		break;
	}
}

void	LTable::popItem(LTItem* item) {
	switch(item->score) {
	case SCORE_BEST:
		m_mapBestItems.erase(item->key);
		break;
	case SCORE_GOOD:
		m_mapGoodItems.erase(item->key);
		break;
	case SCORE_OK:
		m_mapOkItems.erase(item->key);
		break;
	case SCORE_BAD:
		m_mapBadItems.erase(item->key);
		break;
	}
}

LTItem*	LTable::queryInList(std::map<std::string, LTItem*>& list) {
	std::map<std::string, LTItem*>::iterator it;

	for( it = list.begin(); it != list.end(); it++ ) {
		if( it->second->load + it->second->reserve < it->second->load_max ) {
			it->second->reserve++;
			return it->second;
		}
	}

	return NULL;
}
