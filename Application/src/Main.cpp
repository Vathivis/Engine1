#include <Engine1.h>

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Anchor.h"



class Layer1 : public Engine1::Layer {
private:
	std::shared_ptr<Engine1::Shader> m_shader;
	std::shared_ptr<Engine1::VertexArray> m_vertexArray;

	std::shared_ptr<Engine1::Shader> m_blueShader;
	std::shared_ptr<Engine1::VertexArray> m_squareVA;

	std::shared_ptr<Engine1::Shader> m_textureSquareShader;

	std::shared_ptr<Engine1::VertexArray> m_backgroundVA;
	
	Engine1::Texture m_groundPlanTex;
	
	
	//anchors
	std::shared_ptr<Engine1::VertexArray> m_anchorVA;
	Engine1::Texture m_anchorTex;
	std::vector<Anchor> m_anchors;

	//camera
	Engine1::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	glm::vec2 m_mouseScenePos;
	float m_cameraRotation = 0.0f;

	float m_cameraMoveSpeed = 2.0f;
	float m_cameraRotationSpeed = 180.0f;

public:
	Layer1() : Layer("Layer1"), m_groundPlanTex("resources/textures/pudorys-zdi.png"), m_anchorTex("resources/textures/anchor.png"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f) {
	
		m_vertexArray.reset(Engine1::VertexArray::create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Engine1::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Engine1::VertexBuffer::create(vertices, sizeof(vertices)));

		Engine1::BufferLayout layout = {
			{ Engine1::ShaderDataType::Float3, "a_position" },
			{ Engine1::ShaderDataType::Float4, "a_color" }
		};

		vertexBuffer->setLayout(layout);
		m_vertexArray->addVertexBuffer(vertexBuffer);		//volat az po setLayout



		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Engine1::IndexBuffer> indexBuffer;
		indexBuffer.reset(Engine1::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuffer);

		//	na render noveho objektu je potreba nejdrive vytvoreni VertexArray, pak definice vertexu
		//	pote novy VertexBuffer, pak layout tohoto bufferu a setnuti tohoto layoutu pro tento buffer 
		//	dalsi pridame VertexBuffer do VertexArray, zbyvaji indexy -> definice indexu, pak novy IndexBuffer
		//	ten setneme pro VertexArray, zbyva uz jen vykreslit - nabindovat prislusny shader a vertex array
		//	nakonec glDrawElements, kde pocet indexu je VertexArray->getIndexBuffer()->getCount()

		m_squareVA.reset(Engine1::VertexArray::create());

		float squareVertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f, 
			 0.5f, -0.5f, 0.0f, 
			 0.5f,  0.5f, 0.0f,	
			-0.5f,  0.5f, 0.0f 
		};

		std::shared_ptr<Engine1::VertexBuffer> squareVB;
		squareVB.reset(Engine1::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		Engine1::BufferLayout squareVBLayout = {
			{ Engine1::ShaderDataType::Float3, "a_position" }
		};
		squareVB->setLayout(squareVBLayout);
		m_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine1::IndexBuffer> squareIB;
		squareIB.reset(Engine1::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_squareVA->setIndexBuffer(squareIB);


		//background texture
		m_backgroundVA.reset(Engine1::VertexArray::create());

		float groundW = m_groundPlanTex.getWidth();
		float groundH = m_groundPlanTex.getHeight();

		groundW = (groundW / 1000) ;
		groundH = (groundH / 1000) ;

		float backgroundVertices[] = {
			-groundW, -groundH, 0.0f, 0.0f, 0.0f,	//left bot
			 groundW, -groundH, 0.0f, 1.0f, 0.0f,	//right bot
			 groundW,  groundH, 0.0f, 1.0f, 1.0f,	//right top
			-groundW,  groundH, 0.0f, 0.0f, 1.0f	//left top
		};

		Engine1::BufferLayout backgroundVBLayout = {
			{ Engine1::ShaderDataType::Float3, "a_position" },
			{ Engine1::ShaderDataType::Float2, "a_texPos" }
		};

		std::shared_ptr<Engine1::VertexBuffer> backgroundVB;
		backgroundVB.reset(Engine1::VertexBuffer::create(backgroundVertices, sizeof(backgroundVertices)));
		backgroundVB->setLayout(backgroundVBLayout);
		m_backgroundVA->addVertexBuffer(backgroundVB);

		uint32_t backgroundIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine1::IndexBuffer> backgroundIB;
		backgroundIB.reset(Engine1::IndexBuffer::create(backgroundIndices, sizeof(backgroundIndices) / sizeof(uint32_t)));
		m_backgroundVA->setIndexBuffer(backgroundIB);

	
		//anchor
		m_anchorVA.reset(Engine1::VertexArray::create());
		float anchorVertices[] = {
			-0.1f, -0.1f, 0.0f, 0.0f, 0.0f,	//left bot
			 0.1f, -0.1f, 0.0f, 1.0f, 0.0f,	//right bot
			 0.1f,  0.1f, 0.0f, 1.0f, 1.0f,	//right top
			-0.1f,  0.1f, 0.0f, 0.0f, 1.0f	//left top
		};

		Engine1::BufferLayout anchorVBLayout = {
			{ Engine1::ShaderDataType::Float3, "a_position" },
			{ Engine1::ShaderDataType::Float2, "a_texPos" }
		};

		std::shared_ptr<Engine1::VertexBuffer> anchorVB;
		anchorVB.reset(Engine1::VertexBuffer::create(anchorVertices, sizeof(anchorVertices)));
		anchorVB->setLayout(anchorVBLayout);
		m_anchorVA->addVertexBuffer(anchorVB);

		uint32_t anchorIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine1::IndexBuffer> anchorIB;
		anchorIB.reset(Engine1::IndexBuffer::create(anchorIndices, sizeof(anchorIndices) / sizeof(uint32_t)));
		m_anchorVA->setIndexBuffer(anchorIB);



		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform mat4 u_viewProjection;

			in vec3 v_position;
			in vec4 v_color;

			void main()
			{
				color = vec4(v_position * 0.5 + 0.5, 1.0);
				color = v_color;
			}
		)";

		m_shader.reset(new Engine1::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec3 v_position;

			void main()
			{
				v_position = a_position;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform mat4 u_viewProjection;


			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_blueShader.reset(new Engine1::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));

		std::string textureSquareShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec2 a_texPos;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec2 v_texPos;

			void main()
			{
				v_texPos = a_texPos;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);	
			}
		)";

		std::string textureSquareShaderFragmentSrc = R"(
			#version 430 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_texPos;

			uniform sampler2D u_texture;

			void main()
			{
				vec4 texColor = texture(u_texture, v_texPos);
				color = texColor;
			}
		)";

		m_textureSquareShader.reset(new Engine1::Shader(textureSquareShaderVertexSrc, textureSquareShaderFragmentSrc));

	}

	void onUpdate(Engine1::Timestep ts) override {

		//else if to prevent cancelling each other
		if (Engine1::Input::isKeyPressed(E1_KEY_D))
			m_cameraPosition.x += m_cameraMoveSpeed * ts;
		else if (Engine1::Input::isKeyPressed(E1_KEY_A))
			m_cameraPosition.x -= m_cameraMoveSpeed * ts;

		if (Engine1::Input::isKeyPressed(E1_KEY_W))
			m_cameraPosition.y += m_cameraMoveSpeed * ts;
		else if (Engine1::Input::isKeyPressed(E1_KEY_S))
			m_cameraPosition.y -= m_cameraMoveSpeed * ts;


		if (Engine1::Input::isKeyPressed(E1_KEY_E))
			m_cameraRotation += m_cameraRotationSpeed * ts;
		else if (Engine1::Input::isKeyPressed(E1_KEY_Q))
			m_cameraRotation -= m_cameraRotationSpeed * ts;


		Engine1::RenderCommand::setClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Engine1::RenderCommand::clear();

		m_camera.setPosition(m_cameraPosition);
		m_camera.setRotation(m_cameraRotation);



		auto [x, y] = Engine1::Input::getMousePosition();

		//denormalize camera
		glm::vec3 camPos = m_camera.getPosition();
		camPos.x *=  1280 / 3.2;
		camPos.y *= -720  / 1.8;

		m_mouseScenePos = { x + camPos.x, y + camPos.y};
		//E1_WARN("Mouse pos: {0} {1}", m_mouseScenePos.x, m_mouseScenePos.y);

		Engine1::Renderer::beginScene(m_camera);

		/*glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int j = 0; j < 20; ++j) {
			for (int i = 0; i < 20; ++i) {
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Engine1::Renderer::submit(m_blueShader, m_squareVA, transform);		//blue square
			}
		}*/
			
		Engine1::Renderer::submit(m_shader, m_vertexArray);		//colored triangle


		//bacground
		glm::vec3 pos2(0.0f, 0.0f, 0.0f);
		glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), pos2);
		m_groundPlanTex.bind();
		m_textureSquareShader->uploadUniform1f("u_texture", 0);
		Engine1::Renderer::submit(m_textureSquareShader, m_backgroundVA, transform2);

		//anchors
		glm::mat4 anchorScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.33f));		//needs to change with zoom (or not??)
		for (auto& anchor : m_anchors) {
			glm::vec3 pos(anchor.getPosition().x, anchor.getPosition().y, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * anchorScale;
			m_anchorTex.bind();
			m_textureSquareShader->uploadUniform1f("u_texture", 0);
			Engine1::Renderer::submit(m_textureSquareShader, m_anchorVA, transform);
		}



		
		/*glm::vec3 pos3(normalizedMouseX, normalizedMouseY, 0.0f);
		glm::mat4 transform3 = glm::translate(glm::mat4(1.0f), pos3) * anchorScale;
		m_anchorTex.bind();
		m_textureSquareShader->uploadUniform1f("u_texture", 0);
		Engine1::Renderer::submit(m_textureSquareShader, m_anchorVA, transform3);*/



		Engine1::Renderer::endScene();
	}

	virtual void onImGuiRender() override {

	}

	void onEvent(Engine1::Event& event) override {
		Engine1::EventDispatcher dispatcher(event);

		dispatcher.dispatch<Engine1::KeyPressedEvent>(E1_BIND_EVENT_FN(Layer1::onKeyPressedEvent));

	}

	bool onKeyPressedEvent(Engine1::KeyPressedEvent& event) {

		if (event.getKeyCode() == E1_KEY_SPACE) {
			float normalizedMouseX = -1.6f + 3.2f * m_mouseScenePos.x / 1280;
			float normalizedMouseY = 0.9f - 1.8f * m_mouseScenePos.y / 720;
			Anchor anchor({ normalizedMouseX, normalizedMouseY, 0.0f });
			m_anchors.push_back(anchor);
		}

		if (event.getKeyCode() == E1_KEY_C) {
			m_cameraPosition = { 0.0f, 0.0f, 0.0f };
		}

		return false;
	}


};

class Sandbox : public Engine1::Application {
public:
	Sandbox() {
		pushLayer(new Layer1());
	}

	~Sandbox() {

	}

};

Engine1::Application* Engine1::CreateApplication() {
	return new Sandbox();
}