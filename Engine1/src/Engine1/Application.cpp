#include "E1pch.h"

#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace Engine1 {

	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::run() {

		WindowResizeEvent e(1280, 720);
		if (e.isInCategory(EventCategoryApplication)) {
			E1_TRACE(e);
		}
		if (e.isInCategory(EventCategoryInput)) {
			E1_TRACE(e);
		}


		while (true);
	}



}