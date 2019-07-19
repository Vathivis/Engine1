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
		

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,		//x, y, z souradnice		//levy roh
			 0.5f, -0.5f, 0.0f,		//pravy roh
			 0.0f,  0.5f, 0.0f		//stred
		};

		m_vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);		//popisujeme data na indexu 0 pro gpu, stride = pocet bytu mezi vertexy
	
		//indexy vertexu -> rika v jakem poradi tyto vertexy vykreslit

		uint32_t indices[3] = { 0, 1, 2 };		//poradi v jakem se kresli vertexy
		m_indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;

			out vec3 v_position;

			void main(){
				v_position = a_position;
				gl_Position = vec4(a_position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_position;

			void main(){
				color = vec4(v_position * 0.5 + 0.5, 1.0);
			}
		)";

		m_shader.reset(new Shader(vertexSrc, fragmentSrc));

	}

	Application::~Application() {}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//hlavni loop programu /////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

	void Application::run() {
		while (m_running) {

			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->bind();
			glBindVertexArray(m_vertexArray);
			glDrawElements(GL_TRIANGLES, m_indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);


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