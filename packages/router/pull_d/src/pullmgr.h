#ifndef _PULLMGR_H_
#define _PULLMGR_H_

#include "config.h"
#include "pullconfig.h"
#include <map>
#include <set>
#include <string>

class NetLoop;
class MsgHandler;
class Producer;
class Consumer;
class Store;
class Msg;
class NetPuller;
class MongoLink;

class PullMgr {
public:
	PullMgr(PullConfig& config);
	~PullMgr();

public:
	int		registerConsumer(int linkid, const std::string& name, const std::string& topic);
	void	deleteConsumer(int linkid);

	void	createTopic(const std::string& topic);
	void	deleteTopic(const std::string& topic);
	bool	hasTopic(const std::string& topic);

	void	push(const std::string& topic, const char* data, int len);
	void	run();

	void	onTimer();

public:
	MsgHandler*	getHandler() { return m_pHandler; }
	NetLoop*	getLooper() { return m_pLooper; }
	NetPuller*	getPuller() { return m_pPuller; }
	MongoLink*	getMongo() { return m_pMongo; }

private:
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;
	Store*		m_pStore;
	NetPuller*	m_pPuller;
	MongoLink*	m_pMongo;
	std::map<std::string, Consumer*>	m_mapConsumers;
	PullConfig	m_config;
};

#endif