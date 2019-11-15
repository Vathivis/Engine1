#include "E1pch.h"

#include "Engine1/Core/Application.h"
#include "Engine1/Core/Log.h"
#include "Engine1/Core/Input.h"

#include "Engine1/Renderer/Renderer.h"

#include "GLFW/glfw3.h"


namespace Engine1 {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		E1_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_window = Window::create();
		m_window->setEventCallback(E1_BIND_EVENT_FN(Application::onEvent));
		m_window->setVSync(true);

		Renderer::init();

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);

	}

	Application::~Application()
	{
		Renderer::shutdown();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//hlavni loop programu /////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

	void Application::run() {
		while (m_running) {
			
			float time = glfwGetTime();		//Platform::getTime
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_minimized) {
				for (Layer* layer : m_layerStack)
					layer->onUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_layerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->End();

			m_window->onUpdate();
		}
	}
	//hlavni loop programu /////////////////////////////////////////////////////////////////////////

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(E1_BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(E1_BIND_EVENT_FN(Application::onWindowResize));

		//E1_CORE_INFO("{0}", e);

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
	}

	//pridani vrstvy mezi overlay vrtvy
	void Application::pushOverlay(Layer* layer) {
		m_layerStack.pushOverlay(layer);
	}

	bool Application::onWindowClose(WindowCloseEvent& e) {
		m_running = false;

		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e) {
		
		if (e.getWidth() == 0 || e.getHeight() == 0) {
			m_minimized = true;
			return false;
		}

		m_minimized = false;
		Renderer::onWindowResize(e.getWidth(), e.getHeight());

		return false;
	}

}