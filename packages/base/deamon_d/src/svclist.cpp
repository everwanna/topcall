#include "svclist.h"
#include "svcinfo.h"

SvcList::SvcList(short svc) : m_nSvc(svc)
{
	
}

SvcList::~SvcList() {
}

void	SvcList::add(const std::string& name, const std::string& ip, short port, short svid, int isp, int area, int load, int load_max, int stamp) {	
	std::map<std::string, SvcInfo*>::iterator it = m_mapSvcs.find(name);
	if( it == m_mapSvcs.end() ) {
		SvcInfo* info = new SvcInfo(name, ip, port, svid, isp, area, load, load_max, stamp);
		m_mapSvcs[name] = info;
	} else {
		it->second->setIP(ip);
		it->second->setPort(port);
		it->second->setSvid(svid);
		it->second->setIsp(isp);
		it->second->setArea(area);
		it->second->setLoad(load);
		it->second->setLoadMax(load_max);
		it->second->setStamp(stamp);
	}
}

void	SvcList::getSvcInfos(PDQueryRes& res) {
	for(std::map<std::string, SvcInfo*>::iterator it = m_mapSvcs.begin(); it != m_mapSvcs.end(); it++ ) {
		SvcInfo* sinfo = it->second;
		if( sinfo != NULL ) {
			PDSvcInfo info;
			info.svid = sinfo->getSvid();
			info.ip = sinfo->getIP();
			info.port = sinfo->getPort();
			info.isp = sinfo->getIsp();
			info.area = sinfo->getArea();
			info.load = sinfo->getLoad();
			info.load_max = sinfo->getLoadMax();

			res.infos.push_back(info);
		}
	}
}

void	SvcList::removeExpire(int time) {
	SvcInfo* info = NULL;
	for(std::map<std::string, SvcInfo*>::iterator it = m_mapSvcs.begin(); it != m_mapSvcs.end(); it++ ) {
		if( it->second->getStamp() + TIMER_EXPIRE <= time ) {
			info = it->second;
			m_mapSvcs.erase(it);
			LOG(TAG_DEAMON, "remove expired svc, svid=%d, ip=%s, port=%d, isp=%d, area=%d", info->getSvid(), info->getIP().c_str(), info->getPort(),
				info->getIsp(), info->getArea());
			break;	//expire 1 server each time.
		}
	}
}

