#include <Engine1.h>

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/fast_square_root.hpp"
#include "glm/glm.hpp"

#include "Anchor.h"
#include "Scale.h"
#include "Node.h"

//TEMPORARY
#include "glad/glad.h"



class Layer1 : public Engine1::Layer {
private:
	//std::shared_ptr<Engine1::Shader> m_shader;
	//std::shared_ptr<Engine1::VertexArray> m_vertexArray;

	std::shared_ptr<Engine1::Shader> m_flatColorShader;
	std::shared_ptr<Engine1::VertexArray> m_squareVA;

	std::shared_ptr<Engine1::Shader> m_textureSquareShader;

	//background/groundplan
	std::shared_ptr<Engine1::VertexArray> m_backgroundVA;
	Engine1::Texture m_groundPlanTex;
	
	
	//anchors
	std::shared_ptr<Engine1::VertexArray> m_anchorVA;
	Engine1::Texture m_anchorTex;
	std::vector<Anchor> m_anchors;

	bool m_invertLines = false;
	int m_anchorIndex = 0;

	//nodes
	std::shared_ptr<Engine1::VertexArray> m_nodeVA;
	Engine1::Texture m_nodeTex;
	std::vector<Node> m_nodes;

	//scale
	std::shared_ptr<Engine1::VertexArray> m_scaleVA;
	Engine1::Texture m_scaleTex;
	std::unique_ptr<Scale> m_scale;

	bool m_showScale = false;
	

	//camera
	Engine1::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	glm::vec2 m_mouseScenePos;
	float m_cameraRotation = 0.0f;

	float m_cameraMoveSpeed = 2.0f;
	float m_cameraRotationSpeed = 180.0f;

	bool show = true;


public:
	Layer1() : Layer("Layer1"), m_groundPlanTex("resources/textures/pudorys-zdi.png"), m_anchorTex("resources/textures/anchor.png"), m_scaleTex("resources/textures/meritko.png"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f) {
	
		
		//m_scale->setPosition({ -1.3f, 0.8f, 0.0f });

		/*m_vertexArray.reset(Engine1::VertexArray::create());

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
		m_vertexArray->setIndexBuffer(indexBuffer);*/

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	na render noveho objektu je potreba nejdrive vytvoreni VertexArray, pak definice vertexu
		//	pote novy VertexBuffer, pak layout tohoto bufferu a setnuti tohoto layoutu pro tento buffer 
		//	dalsi pridame VertexBuffer do VertexArray, zbyvaji indexy -> definice indexu, pak novy IndexBuffer
		//	ten setneme pro VertexArray, zbyva uz jen vykreslit - nabindovat prislusny shader a vertex array
		//	nakonec glDrawElements, kde pocet indexu je VertexArray->getIndexBuffer()->getCount()
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		/*m_squareVA.reset(Engine1::VertexArray::create());

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
		m_squareVA->setIndexBuffer(squareIB);*/


		//background texture/////////////////////////////////////////////////////////////////////////////////////////////////
		m_backgroundVA.reset(Engine1::VertexArray::create());

		float groundW = m_groundPlanTex.getWidth();
		float groundH = m_groundPlanTex.getHeight();

		groundW = groundW / 1280 * 1.6;
		groundH = groundH / 720 * 0.9;

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

	
		//anchor/////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

		//scale/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_scaleVA.reset(Engine1::VertexArray::create());

		float scaleW = m_scaleTex.getWidth();
		float scaleH = m_scaleTex.getHeight();

		m_scale = std::make_unique<Scale>(Scale(scaleW, scaleH, { 480.0f, 80.0f, 0.0f }));

		scaleW = scaleW / 1280 * 1.6;
		scaleH = scaleH / 720 * 0.9;


		float scaleVertices[] = {
			-scaleW, -scaleH, 0.0f, 0.0f, 0.0f,	//left bot
			 scaleW, -scaleH, 0.0f, 1.0f, 0.0f,	//right bot
			 scaleW,  scaleH, 0.0f, 1.0f, 1.0f,	//right top
			-scaleW,  scaleH, 0.0f, 0.0f, 1.0f	//left top
		};

		Engine1::BufferLayout scaleVBLayout = {
			{ Engine1::ShaderDataType::Float3, "a_position" },
			{ Engine1::ShaderDataType::Float2, "a_texPos" }
		};

		std::shared_ptr<Engine1::VertexBuffer> scaleVB;
		scaleVB.reset(Engine1::VertexBuffer::create(scaleVertices, sizeof(scaleVertices)));
		scaleVB->setLayout(scaleVBLayout);
		m_scaleVA->addVertexBuffer(scaleVB);

		uint32_t scaleIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Engine1::IndexBuffer> scaleIB;
		scaleIB.reset(Engine1::IndexBuffer::create(scaleIndices, sizeof(scaleIndices) / sizeof(uint32_t)));
		m_scaleVA->setIndexBuffer(scaleIB);


		/*std::string vertexSrc = R"(
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

		m_shader.reset(new Engine1::Shader(vertexSrc, fragmentSrc));*/

		std::string flatColorShaderVertexSrc = R"(
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

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec4 u_color;

			void main()
			{
				color = u_color;
			}
		)";

		m_flatColorShader.reset(new Engine1::Shader(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

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
		camPos.x *= 1280 / 3.2;
		camPos.y *= -720 / 1.8;

		m_mouseScenePos = { x + camPos.x, y + camPos.y };

		

		
		//E1_WARN("Mouse pos: {0} {1}", m_mouseScenePos.x, m_mouseScenePos.y);

		Engine1::Renderer::beginScene(m_camera);

		/*glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		for (int j = 0; j < 20; ++j) {
			for (int i = 0; i < 20; ++i) {
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				if (i % 2 == 0)
					m_flatColorShader->uploadUniform4f("u_color", redColor);
				else
					m_flatColorShader->uploadUniform4f("u_color", blueColor);
				
				Engine1::Renderer::submit(m_flatColorShader, m_squareVA, transform);		//blue square
			}
		}*/
			
		//Engine1::Renderer::submit(m_shader, m_vertexArray);		//colored triangle


		//background
		glm::vec3 pos2(0.0f, 0.0f, 0.0f);
		glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), pos2);
		m_groundPlanTex.bind();
		m_textureSquareShader->uploadUniform1f("u_texture", 0);		//weird on intel gpu
		Engine1::Renderer::submit(m_textureSquareShader, m_backgroundVA, transform2);

		//anchors
		//glm::mat4 anchorScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.33f));		//needs to change with zoom (or not??)
		for (auto& anchor : m_anchors) {
			//glm::vec3 pos(anchor.getPosition().x, anchor.getPosition().y, 0.0f);
			anchor.setScale(glm::scale(glm::mat4(1.0f), glm::vec3(0.33f)));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), anchor.getPosition()) * anchor.getScale();
			m_anchorTex.bind();
			m_textureSquareShader->uploadUniform1f("u_texture", 0);
			Engine1::Renderer::submit(m_textureSquareShader, m_anchorVA, transform);
		}

		//scale
		
		if (m_showScale) {	
			m_scale->setScale(glm::vec3(m_scale->getCurrentWidth() / m_scale->getWidth(), 1.0f, 1.0f));
			glm::mat4 scaleTransform = glm::translate(glm::mat4(1.0f), m_scale->getPosition()) * m_scale->getScale();
			m_scaleTex.bind();
			m_textureSquareShader->uploadUniform1f("u_texture", 0);
			Engine1::Renderer::submit(m_textureSquareShader, m_scaleVA, scaleTransform);
		}

		Engine1::Renderer::endScene();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void onImGuiRender() override {

		static float f1 = m_scale->getWidth();
		//static float f2 = m_scale->getScenePosition().x;
		static glm::vec2 pos;
		static float anchorXpos = 0.0f;
		static float anchorYpos = 0.0f;
		static float north, south, west, east;
		static glm::vec4 anchorWalls;
		unsigned char mouseCol[4];
		auto [xx, yy] = Engine1::Input::getMousePosition();

		ImGui::Begin("Debug");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Number of anchors: %d", m_anchors.size());
		ImGui::Text("Mouse scene position: %f %f", m_mouseScenePos.x, m_mouseScenePos.y);
		ImGui::Text("Mouse screen position: %f %f", xx, yy);
		ImGui::Text("Camera position: %f %f", m_camera.getPosition().x * 1280 / 3.2, m_camera.getPosition().y * -720 / 1.8);
		glReadPixels(xx, 720 - yy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, mouseCol);		//change 720
		ImGui::Text("Color on mouse pos: R:%u G:%u B:%u A:%u", mouseCol[0], mouseCol[1], mouseCol[2], mouseCol[3]);
		if (ImGui::Checkbox("Show Scale", &m_showScale)) {}
		ImGui::Text("Scale width: %f  height: %f", m_scale->getWidth(), m_scale->getHeight());
		ImGui::Text("Meter in pixels: %f", m_scale->getMeter());
		ImGui::End();

		//right click
		static bool found = false;
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseClicked(1)) {
			pos = m_mouseScenePos;

			//right click on anchor
			float distance = 0;
			for (int i = 0; i < m_anchors.size(); ++i) {
				glm::vec2 anchPos = m_anchors[i].getScenePosition();
				distance = glm::distance(anchPos, pos);		//can change to fastDistance, but less accurate
				if (distance < m_anchors[i].getRadius()) {
					ImGui::OpenPopup("anchorRightClick");
					m_anchorIndex = i;
					found = true;
					break;
				}
			}

			//right click on background
			if (!found) {
				ImGui::OpenPopup("backgroundRightClick");
			}

		}	


		

		//mouse hold
		static bool found2 = false;
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseDragging(0)) {
			float distance = 0;
			pos = m_mouseScenePos;

			//the position of the scale
			if (m_showScale && m_mouseScenePos.x >= m_scale->getScenePosition().x - m_scale->getWidth() / 2 && m_mouseScenePos.x <= m_scale->getScenePosition().x + m_scale->getWidth() / 2
				&& m_mouseScenePos.y >= m_scale->getScenePosition().y - m_scale->getHeight() / 2 && m_mouseScenePos.y <= m_scale->getScenePosition().y + m_scale->getHeight() / 2) {

				m_scale->setPosition({ m_mouseScenePos.x, m_mouseScenePos.y, 0.0f });
			}
			else {
				for (int i = 0; i < m_anchors.size(); ++i) {
					glm::vec2 anchPos = m_anchors[i].getScenePosition();
					distance = glm::distance(anchPos, pos);		//can change to fastDistance, but less accurate
					if (distance < m_anchors[i].getRadius()) {
						m_anchorIndex = i;
						found2 = true;
						break;
					}
				}

				if (found2 && !m_anchors[m_anchorIndex].getLock()) {
					m_anchors[m_anchorIndex].setPosition({ m_mouseScenePos.x, m_mouseScenePos.y, 0.0f });
				}



			}




		}
		else
			found2 = false;


		//double click on anchor or scale
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseDoubleClicked(0)) {
			pos = m_mouseScenePos;
			float distance = 0;
			
			if (m_showScale && pos.x >= m_scale->getScenePosition().x - m_scale->getWidth() / 2 && pos.x <= m_scale->getScenePosition().x + m_scale->getWidth() / 2
				&& pos.y >= m_scale->getScenePosition().y - m_scale->getHeight() / 2 && pos.y <= m_scale->getScenePosition().y + m_scale->getHeight() / 2) {
				ImGui::OpenPopup("scaleClick");
			}
			else {
				for (int i = 0; i < m_anchors.size(); ++i) {
					glm::vec2 anchPos = m_anchors[i].getScenePosition();
					distance = glm::distance(anchPos, pos);		//can change to fastDistance, but less accurate
					if (distance < m_anchors[i].getRadius()) {
						ImGui::OpenPopup("anchorClick");
						anchorXpos = m_anchors[i].getPosition().x;
						anchorYpos = m_anchors[i].getPosition().y;
						m_anchorIndex = i;
						anchorWalls = getAnchorAllWallDistance(m_anchors[m_anchorIndex]);
						break;
					}
				}
			}
		}

		
		if (ImGui::BeginPopup("scaleClick")) {
			ImGui::Text("Width: %f", m_scale->getCurrentWidth());
			ImGui::Text("Height: %f", m_scale->getCurrentHeight());

			//static float f0 = 0.001f;
			ImGui::InputFloat("", &f1, 0.1f, 1.0f, "%.2f");
			ImGui::SameLine();
			ImGui::Text("set width");
			if (ImGui::IsItemActive() || ImGui::IsItemHovered())
				ImGui::SetTooltip("Hold CONTROL to increase by 1");
			//ImGui::DragFloat("set width", &f1, 0.05f);

			//ImGui::SliderFloat("float", &f2, m_scale->getScenePosition().x - 20.0f, m_scale->getScenePosition().x + 20.0f);

			m_scale->setWidth(f1);
			//m_scale->setPosition({ f2, m_scale->getScenePosition().y, 0.0f });

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("anchorClick")) {
			if (ImGui::Checkbox("Invert Lines", &m_invertLines)) {}

			//m_castRays = true;
			
			

			ImGui::Text("Meters from northern wall: %.3f", anchorWalls.x);
			ImGui::Text("Meters from southern wall: %.3f", anchorWalls.y);
			ImGui::Text("Meters from western wall: %.3f", anchorWalls.z);
			ImGui::Text("Meters from eastern wall: %.3f", anchorWalls.w);


			

			ImGui::EndPopup();
		}


		if (ImGui::BeginPopup("backgroundRightClick")) {
			if (ImGui::Button("Add Anchor")) {
				addAnchor(pos);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("anchorRightClick")) {

			//lock position
			bool lock = m_anchors[m_anchorIndex].getLock();
			if (ImGui::Checkbox("Lock", &lock)) {
				m_anchors[m_anchorIndex].setLock(lock);
			}

			//delete anchor
			if (ImGui::Button("Delete anchor")) {
				m_anchors.erase(m_anchors.begin() + m_anchorIndex);
				ImGui::CloseCurrentPopup();
			}

			found = false;
			ImGui::EndPopup();
		}



		ImGui::ShowDemoWindow(&show);
	}


	void onEvent(Engine1::Event& event) override {
		Engine1::EventDispatcher dispatcher(event);

		dispatcher.dispatch<Engine1::KeyPressedEvent>(E1_BIND_EVENT_FN(Layer1::onKeyPressedEvent));
		//dispatcher.dispatch<Engine1::MouseButtonPressedEvent>(E1_BIND_EVENT_FN(Layer1::onMouseButtonPressedEvent));

	}

	bool onKeyPressedEvent(Engine1::KeyPressedEvent& event) {

		if (event.getKeyCode() == E1_KEY_SPACE) {
			addAnchor(m_mouseScenePos);
		}

		if (event.getKeyCode() == E1_KEY_C) {
			m_cameraPosition = { 0.0f, 0.0f, 0.0f };
			m_cameraRotation = 0.0f;
		}

		return false;
	}

	void addAnchor(const glm::vec2& position) {
		Anchor anchor({ position.x, position.y, 0.0f });
		m_anchors.push_back(anchor);
	}

	//0 = north, 1 = south, 2 = west, 3 = east
	float getAnchorWallDistance(const Anchor& anchor, int direction = 0) {
		unsigned char pick_col[4];
		glm::vec3 camPos = m_camera.getPosition();
		camPos.x *= 1280 / 3.2;
		camPos.y *= 720 / 1.8;

		//change 720 to window height, change sceneposition to window position, change readpixels to be integrated in Engine1
		if (direction == 0) {
			for (int i = 0; i < 1000; ++i) {
				glReadPixels(anchor.getScenePosition().x - camPos.x, 720 - anchor.getScenePosition().y + i - camPos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pick_col);
				if (!(pick_col[0] > 225 && pick_col[1] > 225 && pick_col[2] > 225) && pick_col[2] < 220) {
					return (float)i;
				}
			}
		}
		else if (direction == 1) {
			for (int i = 0; i < 1000; ++i) {
				glReadPixels(anchor.getScenePosition().x - camPos.x, 720 - anchor.getScenePosition().y - i - camPos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pick_col);
				if (!(pick_col[0] > 220 && pick_col[1] > 220 && pick_col[2] > 220) && pick_col[2] < 220) {
					return (float)i;
				}
			}
		}
		else if (direction == 2) {
			for (int i = 0; i < 1000; ++i) {
				glReadPixels(anchor.getScenePosition().x - i - camPos.x, 720 - anchor.getScenePosition().y - camPos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pick_col);
				if (!(pick_col[0] > 220 && pick_col[1] > 220 && pick_col[2] > 220) && pick_col[2] < 220) {
					return (float)i;
				}
			}
		}
		else if (direction == 3) {
			for (int i = 0; i < 1000; ++i) {
				glReadPixels(anchor.getScenePosition().x + i - camPos.x, 720 - anchor.getScenePosition().y - camPos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pick_col);
				if (!(pick_col[0] > 220 && pick_col[1] > 220 && pick_col[2] > 220) && pick_col[2] < 220) {
					return (float)i;
				}
			}
		}
		

		return -1.0f;
	}

	glm::vec4 getAnchorAllWallDistance(const Anchor& anchor) {
		glm::vec4 res;
		res.x = getAnchorWallDistance(anchor, 0);
		res.y = getAnchorWallDistance(anchor, 1);
		res.z = getAnchorWallDistance(anchor, 2);
		res.w = getAnchorWallDistance(anchor, 3);

		return res;
	}
	

};

/*class Overlay1 : public Engine1::Layer {
private:
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:

	Overlay1() : ImGuiLayer() {
		
	}

	void onUpdate(Engine1::Timestep ts) override {

		//this->Begin();
		
		/*static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();


		//this->End();

	}

	virtual void onImGuiRender() override {


	}

	void onEvent(Engine1::Event& event) override {
		Engine1::EventDispatcher dispatcher(event);

	}


};*/

class Sandbox : public Engine1::Application {
public:
	Sandbox() {
		pushLayer(new Layer1());
		//pushOverlay(new Overlay1());
	}

	~Sandbox() {

	}

};

Engine1::Application* Engine1::CreateApplication() {
	return new Sandbox();
}