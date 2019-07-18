#include "E1pch.h"

#include "Application.h"
#include "Engine1/Log.h"
#include "Input.h"

#include <glad/glad.h>

#include "glm/glm.hpp"

namespace Engine1 {

//makro na event bind na usetreni mista
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		E1_CORE_ASSERT(!s_Instance, "Application already exists");		//kontrola jednotnosti
		s_Instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FN(onEvent));

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);

		glGenBuffers(1, &m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);


		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,		//x, y, z souradnice		//levy roh
			 0.5f, -0.5f, 0.0f,		//pravy roh
			 0.0f,  0.5f, 0.0f		//stred
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);		//popisujeme data na indexu 0 pro gpu, stride = pocet bytu mezi vertexy

		glGenBuffers(1, &m_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);		//indexy vertexu -> rika v jakem poradi tyto vertexy vykreslit

		unsigned int indices[3] = { 0, 1, 2 };		//poradi v jakem se kresli vertexy
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

		//shader

	}

	Application::~Application() {}

	//hlavni loop programu -----------------------------------------------------------------------------
	void Application::run() {
		while (m_running) {

			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);


			glBindVertexArray(m_vertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);


			for (Layer* layer : m_layerStack)
				layer->onUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_layerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->End();

			m_window->onUpdate();
		}
	}
	//hlavni loop programu -----------------------------------------------------------------------------

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

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

}