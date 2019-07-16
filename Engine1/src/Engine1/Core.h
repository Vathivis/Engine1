#pragma once

//makro pro dll
#ifdef E1_PLATFORM_WINDOWS
	//makro pouze pro engine, ne cely projekt
	#ifdef E1_BUILD_DLL
		#define ENGINE1_API __declspec(dllexport)
	#else
		#define ENGINE1_API __declspec(dllimport)
	#endif

#endif

#define BIT(x) (1 << x)