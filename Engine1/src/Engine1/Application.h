#pragma once

#include "Core.h"

#include "Window.h"
#include "Events/Event.h"
#include "LayerStack.h"
#include "Engine1/Events/ApplicationEvent.h"

namespace Engine1 {

	class ENGINE1_API Application {
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
		LayerStack m_layerStack;

	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);
	
	private:
		bool onWindowClose(WindowCloseEvent& e);
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}