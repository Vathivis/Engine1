#include "E1pch.h"
#include "Engine1/Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine1 {


	ref<spdlog::logger> Log::s_coreLogger;
	ref<spdlog::logger> Log::s_clientLogger;

	void Log::init() {
		//nastaveni barev pro druhy logu
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_coreLogger = spdlog::stdout_color_mt("ENGINE1");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("IPS");
		s_clientLogger->set_level(spdlog::level::trace); 
	}

}