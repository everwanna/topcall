#include "dispmgr.h"
#include "inifile.h"
#include "dispconfig.h"

int main(int argc, char* argv[])
{
	char* inifile = "dispatcher_d.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG(TAG_DISPATCHER, "dispatcher_d, fail to open %s.", inifile);
		return -1;
	}

	DispConfig config;
	config.name = ini.getString("dispatcher", "name");
	config.ip = ini.getString("dispatcher", "ip");
	config.port = ini.getInt("dispatcher", "port");
	config.isp = ini.getInt("dispatcher", "isp");
	config.area = ini.getInt("dispatcher", "area");
	config.router_ip = ini.getString("router", "disp_ip");
	config.router_port = ini.getInt("router", "disp_port");

	if( config.name.length() == 0 ||
		config.ip.length() == 0 ||
		config.port == 0 ||
		config.isp == 0 ||
		config.area == 0) {
		LOG(TAG_DISPATCHER, "dispatcher_d, the config file is invalid.");
		return -1;
	}

	LOG(TAG_DISPATCHER, "dispatcher_d, running at configuration: ");
	LOG(TAG_DISPATCHER, "	name    = %s ", config.name.c_str());
	LOG(TAG_DISPATCHER, "	ip      = %s ", config.ip.c_str());
	LOG(TAG_DISPATCHER, "	port    = %d ", config.port);
	LOG(TAG_DISPATCHER, "	area    = %d ", config.area);
	LOG(TAG_DISPATCHER, "	router_ip   = %s ", config.router_ip.c_str());
	LOG(TAG_DISPATCHER, "	router_port = %d ", config.router_port);

	DispMgr* mgr = new DispMgr(config);
	mgr->run();

	return 0;
}
