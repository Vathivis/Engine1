#pragma once

#include "Core.h"

#include "Window.h"
#include "Events/Event.h"
#include "LayerStack.h"
#include "Engine1/Events/ApplicationEvent.h"

#include "Engine1/ImGui/ImGuiLayer.h"


namespace Engine1 {

	class Application {
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
		LayerStack m_layerStack;
		static Application* s_Instance;		//instance aplikace - muze byt pouze jedna v programu
		ImGuiLayer* m_ImGuiLayer;

	public:
		Application();
		virtual ~Application() = default;

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		static inline Application& get() { return *s_Instance; }
		inline Window& getWindow() { return *m_window; }
	
	private:
		bool onWindowClose(WindowCloseEvent& e);
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}