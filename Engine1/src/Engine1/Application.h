#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Engine1 {

	

	class ENGINE1_API Application {
	private:


	public:

		Application();
		virtual ~Application();

		void run();


	};

	//definovane v klientovi
	Application* CreateApplication();

}