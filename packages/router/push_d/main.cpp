// media_push_d.cpp : Defines the entry point for the console application.
//
#include "pushmgr.h"
#include "pushconfig.h"
#include "inifile.h"

int main(int argc, char* argv[])
{
	char* inifile = "push_d.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG(TAG_PUSH, "push_d, fail to open %s.", inifile);
		return -1;
	}

	PushConfig config;
	config.name = ini.getString("push", "name");
	config.ip = ini.getString("push", "ip");
	config.port = ini.getInt("push", "port");	

	config.mongo_enable = ini.getInt("mongo", "enable");
	config.mongo_dbname = ini.getString("mongo", "dbname");
	config.mongo_ip = ini.getString("mongo", "ip");
	config.mongo_port = ini.getInt("mongo", "port");	

	if( config.name.length() == 0 ||
		config.ip.length() == 0 ||
		config.port == 0) {
		LOG(TAG_PUSH, "push_d, the config file is invalid.");
		return -1;
	}

	LOG(TAG_PUSH, "push_d, running at configuration: ");
	LOG(TAG_PUSH, "	name    = %s ", config.name.c_str());
	LOG(TAG_PUSH, "	ip      = %s ", config.ip.c_str());
	LOG(TAG_PUSH, "	port    = %d ", config.port);

	PushMgr* mgr = new PushMgr(config);
	mgr->run();

	return 0;
}

