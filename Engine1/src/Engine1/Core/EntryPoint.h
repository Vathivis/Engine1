#pragma once
#include "Engine1/Core/Core.h"

#ifdef E1_PLATFORM_WINDOWS


extern Engine1::Application* Engine1::CreateApplication();


int main(int argc, char** argv) {

	Engine1::Log::init();
	E1_CORE_WARN("Initialized log");
	//E1_INFO("yes");

	auto app = Engine1::CreateApplication();
	app->run();
	delete app;
	

}



#endif