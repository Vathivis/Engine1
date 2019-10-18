#pragma once

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Engine1 {
	/*
		trida pro logovani eventu, warning, info, error ...
	*/
	class ENGINE1_API Log {
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;

	public:

		Log();
		~Log();

		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }

	};

}

//core makra pro logging
#define E1_CORE_CRITICAL(...)  ::Engine1::Log::getCoreLogger()->critical(__VA_ARGS__)
#define E1_CORE_ERROR(...)  ::Engine1::Log::getCoreLogger()->error(__VA_ARGS__)
#define E1_CORE_WARN(...)   ::Engine1::Log::getCoreLogger()->warn(__VA_ARGS__)
#define E1_CORE_INFO(...)   ::Engine1::Log::getCoreLogger()->info(__VA_ARGS__)
#define E1_CORE_TRACE(...)  ::Engine1::Log::getCoreLogger()->trace(__VA_ARGS__)

//klient makra pro logging
#define E1_CRITICAL(...)  ::Engine1::Log::getClientLogger()->critical(__VA_ARGS__)
#define E1_ERROR(...)  ::Engine1::Log::getClientLogger()->error(__VA_ARGS__)
#define E1_WARN(...)   ::Engine1::Log::getClientLogger()->warn(__VA_ARGS__)
#define E1_INFO(...)   ::Engine1::Log::getClientLogger()->info(__VA_ARGS__)
#define E1_TRACE(...)  ::Engine1::Log::getClientLogger()->trace(__VA_ARGS__)
