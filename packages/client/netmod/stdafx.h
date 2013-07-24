// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#ifndef WINVER					// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0500			//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT			// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINDOWS			// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0500	//为 Windows Me 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_IE				// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0600		//为 IE 5.0 及更新版本改变为适当的值。
#endif

// Windows 头文件:
#include <windows.h>