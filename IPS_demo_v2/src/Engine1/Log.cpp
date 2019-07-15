#include "Log.h"

namespace Engine1 {


	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger;

	void Log::init() {
		//nastaveni barev pro druhy logu
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_coreLogger = spdlog::stdout_color_mt("ENGINE1");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("IPS");
		s_clientLogger->set_level(spdlog::level::trace); 
	}

}