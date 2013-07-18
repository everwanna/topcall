#include "blacklist.h"


struct BLItem {
	int		reason;
	int		ts_start;
	int		ts_period;
};

BlackList::BlackList() {
}

BlackList::~BlackList() {
}

/**
 * [TBD] when to remove the BLItem, need another timer check?
 */
void	BlackList::addAttach(int ip, short port, int reason, int time) {
	BLItem* item = new BLItem();
	item->reason = reason;
	item->ts_start = time;
	item->ts_period = 0;

	m_mapBLItems[ip][port] = item;
}

int		BlackList::isBlocked(int ip, short port) {
	std::map<int, std::map<short, BLItem*> >::iterator it = m_mapBLItems.find(ip);
	if( it == m_mapBLItems.end() )
		return BL_REASON_NO;

	std::map<short, BLItem*>::iterator it2 = it->second.find(port);
	if( it2 == it->second.end() )
		return BL_REASON_NO;

	return it2->second->reason;
}

void	BlackList::removeExpire(int time) {
	std::map<int, std::map<short, BLItem*> >::iterator it;
	std::map<short, BLItem*>::iterator it2;
	BLItem*  item = NULL;

	//only remove one item each time:
	for( it = m_mapBLItems.begin(); it != m_mapBLItems.end(); it++ ) {
		for( it2 = it->second.begin(); it2 != it->second.end(); it2++ ) {
			if( it2->second->ts_start + BLACKLIST_PERIOD > time ) {
				//it's time to release the blocked item:
				item = it2->second;
				it->second.erase(it2);
				delete item;
				return;
			}
		}
	}
}
