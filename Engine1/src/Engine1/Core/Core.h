#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#error "x86 Builds are not supported!"
	#else
		/* Windows x86 */
		#define E1_PLATFORM_WINDOWS
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define E1_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define E1_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define E1_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define E1_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection

// DLL support
#ifdef E1_PLATFORM_WINDOWS
	#if E1_DYNAMIC_LINK
		#ifdef E1_BUILD_DLL
			#define ENGINE1_API __declspec(dllexport)
		#else
			#define ENGINE1_API __declspec(dllimport)
		#endif
	#else
		#define ENGINE1_API
	#endif
#else
	#error "Engine1 only supports Windows!"
#endif // End of DLL support

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
	template<typename T, typename ... Args>
	constexpr scope<T> createScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	//ref = shared pointer
	using ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr ref<T> createRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
