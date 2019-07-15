#pragma once

#ifdef E1_PLATFORM_WINDOWS


extern Engine1::Application* Engine1::CreateApplication();


int main(int argc, char** argv) {

	Engine1::Log::init();
	Engine1::Log::getCoreLogger()->warn("Initialized log");
	Engine1::Log::getClientLogger()->info("yes");

	auto ips = Engine1::CreateApplication();
	ips->run();
	delete ips;

}



#endif