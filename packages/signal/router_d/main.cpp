#include "routemgr.h"
#include "inifile.h"
#include "routeconfig.h"

int main(int argc, char* argv[])
{
	char* inifile = "router_d.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG(TAG_ROUTER, "router_d, fail to open %s.", inifile);
		return -1;
	}

	RouteConfig config;
	config.name = ini.getString("router", "name");
	config.ip = ini.getString("router", "ip");
	config.port = ini.getInt("router", "port");
	config.isp = ini.getInt("router", "isp");
	config.area = ini.getInt("router", "area");
	config.router1_ip = ini.getString("network", "router1_ip");
	config.router1_port = ini.getInt("network", "router1_port");
	config.router2_ip = ini.getString("network", "router2_ip");
	config.router2_port = ini.getInt("network", "router2_port");
	config.router3_ip = ini.getString("network", "router3_ip");
	config.router3_port = ini.getInt("network", "router3_port");
	config.router4_ip = ini.getString("network", "router4_ip");
	config.router4_port = ini.getInt("network", "router4_port");

	if( config.name.length() == 0 ||
		config.ip.length() == 0 ||
		config.port == 0 ||
		config.isp == 0 ||
		config.area == 0) {
		LOG(TAG_ROUTER, "router_d, the config file is invalid.");
		return -1;
	}

	LOG(TAG_ROUTER, "router_d, running at configuration: ");
	LOG(TAG_ROUTER, "	name    = %s ", config.name.c_str());
	LOG(TAG_ROUTER, "	ip      = %s ", config.ip.c_str());
	LOG(TAG_ROUTER, "	port    = %d ", config.port);
	LOG(TAG_ROUTER, "	area    = %d ", config.area);

	LOG(TAG_ROUTER, "	router1 = %s ", config.router1_ip.c_str());
	LOG(TAG_ROUTER, "	router1 = %d ", config.router1_port);

	RouteMgr* mgr = new RouteMgr(config);
	mgr->run();

	return 0;
}
