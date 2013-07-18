#ifndef _INIFILE_H_
#define _INIFILE_H_

#include <string>
#include <map>

class IniFile {
public:
	IniFile(const std::string& filename);
	~IniFile();

	typedef std::map<std::string, std::map<std::string, std::string> > map_type;
	typedef std::map<std::string, std::string> value_type;

public:
	int		load();
	std::string	getString( const std::string& section, const std::string& key);
	int		getInt( const std::string& section, const std::string& key );

private:
	std::string	trim(const std::string& str);

public:
	std::map<std::string, std::map<std::string, std::string> > m_mapValues;
	std::string		m_strFileName;
};

#endif
