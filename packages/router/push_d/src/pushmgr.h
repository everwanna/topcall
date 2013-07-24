#ifndef PUSHMGR_H
#define PUSHMGR_H

#include "config.h"
#include "pushconfig.h"
#include <map>
#include <set>
#include <string>

class NetLoop;
class MsgHandler;
class Producer;
class Consumer;
class Store;
class Msg;

class PushMgr {
public:
	PushMgr(PushConfig& config);
	~PushMgr();

public:
	int		registerConsumer(int linkid, const std::string& name, const std::string& topic);
	void	deleteConsumer(int linkid);

	void	createTopic(const std::string& topic);
	void	deleteTopic(const std::string& topic);
	bool	hasTopic(const std::string& topic);

	void	push(const std::string& topic, const std::string& payload);
	void	run();

public:
	MsgHandler*	getHandler() { return m_pHandler; }
	NetLoop*	getLooper() { return m_pLooper; }

private:
	NetLoop*	m_pLooper;
	MsgHandler*	m_pHandler;
	Store*		m_pStore;
	std::map<std::string, Consumer*>	m_mapConsumers;
	PushConfig	m_config;
};

#endif