#include "E1pch.h"
#include "Application.h"

#include "Engine1/Events/ApplicationEvent.h"
#include "Engine1/Log.h"

#include <GLFW/glfw3.h>

namespace Engine1 {

	Application::Application() {
		m_window = std::unique_ptr<Window>(Window::create());
	}

	Application::~Application() {}

	void Application::run() {
		while (m_running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_window->onUpdate();
		}
	}

}