#include "loginmgr.h"
#include "inifile.h"
#include "loginconfig.h"

int main(int argc, char* argv[])
{
	const char* inifile = "login_d.ini";

	if( argc >= 2 ) {
		inifile = argv[1];
	}

	IniFile ini(inifile);
	if( ini.load() != 0 ) {
		LOG(TAG_LOGIN, "login_d, fail to open %s.", inifile);
		return -1;
	}

	LoginConfig config;
	config.name = ini.getString("login", "name");
	config.ip = ini.getString("login", "ip");
	config.port = ini.getInt("login", "port");
	config.seq_high = ini.getInt("login", "seq_high");
	config.isp = ini.getInt("login", "isp");
	config.area = ini.getInt("login", "area");
	config.disp_ip = ini.getString("dispatcher", "ip");
	config.disp_port = ini.getInt("dispatcher", "port");
	config.udb_name = ini.getString("udb", "dbname");
	config.udb_ip = ini.getString("udb", "ip");
	config.udb_port = ini.getInt("udb", "port");

	if( config.name.length() == 0 ||
		config.ip.length() == 0 ||
		config.port == 0 ||
		config.isp == 0 ||
		config.area == 0) {
		LOG(TAG_LOGIN, "login_d, the config file is invalid.");
		return -1;
	}

	LOG(TAG_LOGIN, "login_d, running at configuration: ");
	LOG(TAG_LOGIN, "	name    = %s ", config.name.c_str());
	LOG(TAG_LOGIN, "	ip      = %s ", config.ip.c_str());
	LOG(TAG_LOGIN, "	port    = %d ", config.port);
	LOG(TAG_LOGIN, "	area    = %d ", config.area);
	LOG(TAG_LOGIN, "	master_ip   = %s ", config.disp_ip.c_str());
	LOG(TAG_LOGIN, "	master_port = %d ", config.disp_port);
	LOG(TAG_LOGIN, "	udb_name = %s ", config.name.c_str());
	LOG(TAG_LOGIN, "	udb_ip   = %s ", config.udb_ip.c_str());
	LOG(TAG_LOGIN, "	udb_port = %d ", config.udb_port);

	LoginMgr* mgr = new LoginMgr(config);
	mgr->run();

	return 0;
}
