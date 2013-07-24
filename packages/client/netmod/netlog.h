#ifndef _NETLOG_H
#define _NETLOG_H

#include <iostream>

template<typename T>
inline void NET_LOG(T msg)
{
//	std::count<<"[netmod] " <<msg;
}

template<typename T>
inline void NET_LOG(const std::string& msg, T value)
{
//	std::count<<"[netmod] " << msg << " " << value;
}

template<typename T1, typename T2>
inline void NET_LOG(const std::string& msg, T1 value1, T2 value2)
{
//	std::count<<"[netmod] " << msg << " " << value1 <<", " <<value2;
}

template<typename T1, typename T2, typename T3>
inline void NET_LOG(const std::string& msg, T1 value1, T2 value2, T3 value3)
{
//	std::count<<"[netmod] " << msg << " " << value1 <<", " <<value2 <<", " <<value3;
}

#endif