#pragma once

#include <memory>

//makro pro dll
#ifdef E1_PLATFORM_WINDOWS
#if E1_DYNAMIC_LINK
	//makro pouze pro engine, ne cely projekt
	#ifdef E1_BUILD_DLL
		#define ENGINE1_API __declspec(dllexport)
	#else
		#define ENGINE1_API __declspec(dllimport)
	#endif
#else
	#define ENGINE1_API
#endif
#endif

#ifdef E1_DEBUG
	#define E1_ENABLE_ASSERTS
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

//std::bind makro na usetreni mista
#define E1_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Engine1 {

	template<typename T>
	//scope = unique pointer
	using scope = std::unique_ptr<T>;

	template<typename T>
	//ref = shared pointer
	using ref = std::shared_ptr<T>;


}
