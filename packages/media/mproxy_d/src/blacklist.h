#ifndef BLACKLIST_H
#define BLACKLIST_H

#include "config.h"
#include <map>

#define BL_REASON_NO					100
#define BL_REASON_INVALID_PRODUCER		100
#define BL_REASON_INVALID_CONSUMER		101
#define BL_REASON_INVALID_STREAM		102
#define BL_REASON_INVALID_UID			103
#define BL_REASON_INVALID_PACKET		104
#define BL_REASON_FLOOD					105

struct BLItem;
/**
 * [TBD]
 * There maybe team has such service, let see if I can reuse these services.
 */
class BlackList {
public:
	BlackList();
	~BlackList();

public:
	void	addAttach(int ip, short port, int reason, int time);
	int		isBlocked(int ip, short port);
	void	removeExpire(int time);

private:
	std::map<int, std::map<short, BLItem*> > m_mapBLItems;
};

#endif
