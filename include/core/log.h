#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
/*
inline void LOG(const char* format, ...) {
	static char buf[1024];
	va_list args; 
	va_start(args, format);
	_vsnprintf_s(buf, 1024, format, args); 
	va_end(args); 
	
	printf("%s\r\n", buf);
}
*/

inline void LOG(const char* tag, const char* format, ...) {
	static char buf[1024];
	va_list args; 
	va_start(args, format);
	_vsnprintf_s(buf, 1024, format, args); 
	va_end(args); 
	
	printf(tag);
	printf("%s\r\n", buf);
}

#else
#include <syslog.h>

inline void LOG(const char* tag, const char* format, ...) {
	static char buf[1024];
	va_list args; 
	va_start(args, format);
	vsprintf(buf, format, args); 
	va_end(args); 
	
	//syslog(LOG_USER, "%s %s\r\n", tag, buf);	
	printf("%s%s\r\n", tag, buf);
}

#endif

#endif
