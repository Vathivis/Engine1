#pragma once

#include "Core.h"

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