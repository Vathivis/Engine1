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

//assert makro pouze v debug modu
#ifdef E1_ENABLE_ASSERTS
	#define E1_ASSERT(x, ...) { if(!(x)) { E1_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define E1_CORE_ASSERT(x, ...) { if(!(x)) { E1_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define E1_ASSERT(x, ...)
	#define E1_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)