#include "E1pch.h"
#include "Application.h"

#include "Engine1/Log.h"

#include <glad/glad.h>

namespace Engine1 {

//makro na event bind na usetreni mista
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		E1_CORE_ASSERT(!s_Instance, "Application already exists");		//kontrola jednotnosti
		s_Instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FN(onEvent));
	}

	Application::~Application() {}

	void Application::run() {
		while (m_running) {

			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_layerStack)
				layer->onUpdate();

			m_window->onUpdate();
		}
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

		E1_CORE_INFO("{0}", e);

		//kontrola vrstev od zadu - pri kliknuti na nejake misto chceme tu nejvyssi vrstvu, protoze to je ta kterou uzivatel vidi
		//pokud se event zpracuje konec smycky -> spodni vrstvy nejsou ovlivneny
		for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
		{
			(*--it)->onEvent(e);
			if (e.handled)
				break;
		}
	}

	//pridani vrstvy mezi normalni vrstvy
	void Application::pushLayer(Layer* layer) {
		m_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	//pridani vrstvy mezi overlay vrtvy
	void Application::pushOverlay(Layer* layer) {
		m_layerStack.pushOverlay(layer);
		layer->onAttach();
	}

	bool Application::onWindowClose(WindowCloseEvent& e) {
		m_running = false;

		return true;
	}

}