#pragma once

#ifdef E1_PLATFORM_WINDOWS


extern Engine1::Application* Engine1::CreateApplication();


int main(int argc, char** argv) {

	auto ips = Engine1::CreateApplication();
	ips->run();
	delete ips;

}



#endif