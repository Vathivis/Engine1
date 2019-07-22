#include "E1pch.h"

#include "Application.h"
#include "Engine1/Log.h"
#include "Input.h"

#include "Engine1/Renderer/Renderer.h"

#include "glm/glm.hpp"

namespace Engine1 {

//makro na event bind na usetreni mista
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		E1_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FN(onEvent));

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);

		m_vertexArray.reset(VertexArray::create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));
		
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_position" },
			{ ShaderDataType::Float4, "a_color" }
		};

		vertexBuffer->setLayout(layout);
		m_vertexArray->addVertexBuffer(vertexBuffer);		//volat az po setLayout


		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuffer);

		//	na render noveho objektu je potreba nejdrive vytvoreni VertexArray, pak definice vertexu
		//	pote novy VertexBuffer, pak layout tohoto bufferu a setnuti tohoto layoutu pro tento buffer 
		//	dalsi pridame VertexBuffer do VertexArray, zbyvaji indexy -> definice indexu, pak novy IndexBuffer
		//	ten setneme pro VertexArray, zbyva uz jen vykreslit - nabindovat prislusny shader a vertex array
		//	nakonec glDrawElements, kde pocet indexu je VertexArray->getIndexBuffer()->getCount()

		m_squareVA.reset(VertexArray::create());

		float squareVertices[4 * 3] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		BufferLayout squareVBLayout = {
			{ ShaderDataType::Float3, "a_position" }
		};
		squareVB->setLayout(squareVBLayout);
		m_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_squareVA->setIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = vec4(a_position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_position;
			in vec4 v_color;

			void main()
			{
				color = vec4(v_position * 0.5 + 0.5, 1.0);
				color = v_color;
			}
		)";

		m_shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;

			out vec3 v_position;

			void main()
			{
				v_position = a_position;
				gl_Position = vec4(a_position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_blueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));

	}


	////////////////////////////////////////////////////////////////////////////////////////////////
	//hlavni loop programu /////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

	void Application::run() {
		while (m_running) {

			//lClearColor(0.2f, 0.2f, 0.2f, 1);
			//glClear(GL_COLOR_BUFFER_BIT);

			RenderCommand::setClearColor({ 0.2f, 0.2f, 0.2f, 1 });
			RenderCommand::clear();

			Renderer::beginScene();

			m_blueShader->bind();
			Renderer::submit(m_squareVA);

			m_shader->bind();
			Renderer::submit(m_vertexArray);

			Renderer::endScene();



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