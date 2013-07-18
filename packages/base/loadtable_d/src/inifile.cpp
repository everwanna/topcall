#include "inifile.h"
#include <fstream>
#include <iostream>
#include <string>

IniFile::IniFile(const std::string& filename) : m_strFileName(filename) {
}

IniFile::~IniFile() {
}

int		IniFile::load() {
	std::ifstream ifs(m_strFileName.c_str());
	std::string line;
	std::string section;
	std::string key;
	std::string value;
	int pos = -1;

	if( !ifs.is_open() ) {
		return -1;
	}

	while( getline(ifs, line) ) {
		if( line.length() == 0 ) {
			continue;
		}

		line = trim(line);
		if( line[0] == '[' ) {
			section = trim(line.substr(1, line.length()-2 ));

			std::map<std::string, std::string> values;
			m_mapValues[section] = values;
		} else {
			pos = line.find("=");
			if( pos == -1 ) {
				continue;
			}

			key = trim(line.substr(0, pos));
			value = trim(line.substr(pos+1));

			m_mapValues[section][key] = value;
		}
	}

	return 0;
}

std::string	IniFile::getString( const std::string& section, const std::string& key) {
	map_type::iterator it =  m_mapValues.find(section);
	if( it == m_mapValues.end() )
		return "";

	if( it->second.find(key) == it->second.end() )
		return "";

	return it->second[key];
}

int		IniFile::getInt( const std::string& section, const std::string& key ) {
	std::string svalue = getString(section, key);
	if( svalue.length() == 0 )
		return 0;

	return atoi(svalue.c_str());
}

std::string	IniFile::trim(const std::string& str) {
	int start = 0;
	int end = str.length()-1;

	for( size_t i=0; i<str.length(); i++ ) {
		if( str[i] != ' ' ) {		
			start = i;
			break;
		}
	}
	
	for( size_t i=str.length()-1; i>=0; i-- ) {
		if( str[i] != ' ' && str[i] != '\r' && str[i] != '\n' ) {
			end = i;
			break;
		}
	}

	return str.substr(start, end-start+1);
}

