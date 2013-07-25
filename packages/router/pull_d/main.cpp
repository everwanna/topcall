// media_push_d.cpp : Defines the entry point for the console application.
//
#include "pullmgr.h"
#include "pullconfig.h"
#include "inifile.h"

int main(int argc, char* argv[])
{
	char* inifile = "pull_d.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG(TAG_PULL, "pull_d, fail to open %s.", inifile);
		return -1;
	}

	PullConfig config;	
	config.router_ip = ini.getString("router", "ip");
	config.router_port = ini.getInt("router", "port");	

	config.mongo_dbname = ini.getString("mongo", "dbname");
	config.mongo_ip = ini.getString("mongo", "ip");
	config.mongo_port = ini.getInt("mongo", "port");	

	LOG(TAG_PULL, "pull_d, running at configuration: ");
	LOG(TAG_PULL, "	router    = %s ", config.router_ip.c_str());

	PullMgr* mgr = new PullMgr(config);
	mgr->run();

	return 0;
}

