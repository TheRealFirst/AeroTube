#pragma once

#ifdef PLATFORM_WINDOWS
/*
* #ifdef BUILD_DLL
		#define AT_API __declspec(dllexport)
	#else
		#define AT_API __declspec(dllimport)
	#endif
*/
#else
	#error The engine only supports Windows

#endif