// media_push_d.cpp : Defines the entry point for the console application.
//
#include "pullmgr.h"
#include "pullconfig.h"
#include "inifile.h"

int main(int argc, char* argv[])
{
	char* inifile = "push_d.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG(TAG_PULL, "push_d, fail to open %s.", inifile);
		return -1;
	}

	PullConfig config;
	config.name = ini.getString("push", "name");
	config.ip = ini.getString("push", "ip");
	config.port = ini.getInt("push", "port");	

	if( config.name.length() == 0 ||
		config.ip.length() == 0 ||
		config.port == 0) {
		LOG(TAG_PULL, "push_d, the config file is invalid.");
		return -1;
	}

	LOG(TAG_PULL, "push_d, running at configuration: ");
	LOG(TAG_PULL, "	name    = %s ", config.name.c_str());
	LOG(TAG_PULL, "	ip      = %s ", config.ip.c_str());
	LOG(TAG_PULL, "	port    = %d ", config.port);

	PullMgr* mgr = new PullMgr(config);
	mgr->run();

	return 0;
}

