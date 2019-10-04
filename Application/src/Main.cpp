#include <Engine1.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/fast_square_root.hpp"
#include "glm/glm.hpp"

#include "Anchor.h"
#include "Node.h"
#include "Forklift.h"
#include "Scale.h"
#include "Network/UDPServer.h"
#include "Network/UDPClient.h"

//TEMPORARY opengl
#include "glad/glad.h"

//lokalize node from distances
glm::vec4 calculateFromZ(const glm::vec3& anch1pos, const glm::vec3& anch2pos, const glm::vec3& anch3pos, const glm::mat2& matrix, float v1, float k1d, float k2d, float z) {

	float k1 = k1d - 2 * (anch2pos.z - anch1pos.z) * z;
	float k2 = k2d - 2 * (anch3pos.z - anch1pos.z) * z;

	const float* source = (const float*)glm::value_ptr(matrix);
	float det = glm::determinant(matrix);

	float x = (source[3] * k1 - source[1] * k2) / det;
	float y = (-source[2] * k1 + source[0] * k2) / det;

	float tmp = pow((anch1pos.x - x), 2) + pow((anch1pos.y - y), 2) + pow((anch1pos.z - z), 2) - pow(v1, 2);

	glm::vec4 result({ x, y, z, tmp });

	return result;
}

//glm::vec3 localizeNode(const Anchor& anchor1, const Anchor& anchor2, const Anchor& anchor3, const Node& node) {
glm::vec3 localizeNode(const Anchor & anchor1, const Anchor & anchor2, const Anchor & anchor3, float v1, float v2, float v3) {

	glm::vec3 anch1Pos;
	glm::vec3 anch2Pos;
	glm::vec3 anch3Pos;
	glm::vec3 nodePos;
	
	anch1Pos = anchor1.getScenePosition();	//{ -19, 66, 0 };
	anch2Pos = anchor2.getScenePosition();	//{ 420, 66, 0 };
	anch3Pos = anchor3.getScenePosition();	//{ -19, 340, 0 };
	//nodePos = node.getScenePosition();		//{ 178, 213, 0 };


	// vzdalenosti objektu od majaku vcetne sumu
	//float v1 = sqrt(pow((nodePos.x - anch1Pos.x), 2) + pow((nodePos.y - anch1Pos.y), 2) + pow((nodePos.z - anch1Pos.z), 2)) + 1.45 * rand() / RAND_MAX - 0.125;
	//float v2 = sqrt(pow((nodePos.x - anch2Pos.x), 2) + pow((nodePos.y - anch2Pos.y), 2) + pow((nodePos.z - anch2Pos.z), 2)) - 1.45 * rand() / RAND_MAX - 0.125;
	//float v3 = sqrt(pow((nodePos.x - anch3Pos.x), 2) + pow((nodePos.y - anch3Pos.y), 2) + pow((nodePos.z - anch3Pos.z), 2)) + 1.45 * rand() / RAND_MAX - 0.125;


	//triangulace
	float tmpMat[4] = { 2 * (anch2Pos.x - anch1Pos.x), 2 * (anch2Pos.y - anch1Pos.y), 2 * (anch3Pos.x - anch1Pos.x), 2 * (anch3Pos.y - anch1Pos.y) };
	glm::mat2 matrix = glm::make_mat2(tmpMat);

	

	float k1d = pow(v1, 2) - pow(v2, 2) + pow(anch2Pos.x, 2) - pow(anch1Pos.x, 2) + pow(anch2Pos.y, 2) - pow(anch1Pos.y, 2) + pow(anch2Pos.z, 2) - pow(anch1Pos.z, 2);
	float k2d = pow(v1, 2) - pow(v3, 2) + pow(anch3Pos.x, 2) - pow(anch1Pos.x, 2) + pow(anch3Pos.y, 2) - pow(anch1Pos.y, 2) + pow(anch3Pos.z, 2) - pow(anch1Pos.z, 2);

	// metoda puleni intervalu

	//odhad intervalu pro z - osovou soradnici
	float zmin = 0;
	float zmax = 3;

	float valmin = calculateFromZ(anch1Pos, anch2Pos, anch3Pos, matrix, v1, k1d, k2d, zmin).w;
	float valmax = calculateFromZ(anch1Pos, anch2Pos, anch3Pos, matrix, v1, k1d, k2d, zmax).w;

	//podminka ukonceni
	glm::vec3 pos;
	while (zmax - zmin > 0.005) {

		float zstr = (zmin + zmax) / 2;
		float valstr = calculateFromZ(anch1Pos, anch2Pos, anch3Pos, matrix, v1, k1d, k2d, zstr).w;
		pos = calculateFromZ(anch1Pos, anch2Pos, anch3Pos, matrix, v1, k1d, k2d, zstr);

		if (valstr * valmin > 0) {
			zmin = zstr;
			valmin = valstr;
		}
		else {
			zmax = zstr;
			valmax = valstr;
		}
	}

	return pos;
}



class Layer1 : public Engine1::Layer {
private:
	//Engine1::ref<Engine1::Shader> m_shader;
	//Engine1::ref<Engine1::VertexArray> m_vertexArray;

	Engine1::ref<Engine1::Shader> m_flatColorShader;
	Engine1::ref<Engine1::VertexArray> m_squareVA;
	glm::vec3 m_squareColor = { 0.2f, 0.3f, 0.8f };
	Engine1::ref<Engine1::Texture2D> m_texture;

	//shader library
	Engine1::ShaderLibrary m_shaderLibrary;


	//background/groundplan
	Engine1::ref<Engine1::VertexArray> m_backgroundVA;
	Engine1::ref<Engine1::Texture2D> m_groundPlanWallsTex;
	Engine1::ref<Engine1::Texture2D> m_groundPlanTex;

	bool m_showFurniture = false;
	
	
	//anchors
	Engine1::ref<Engine1::VertexArray> m_anchorVA;
	Engine1::ref<Engine1::Texture2D> m_anchorTex;
	std::vector<Anchor> m_anchors;

	bool m_invertLines = false;
	int m_anchorIndex = 0;

	//nodes
	Engine1::ref<Engine1::VertexArray> m_nodeVA;
	Engine1::ref<Engine1::Texture2D> m_nodeTex;
	std::vector<Node> m_nodes;

	int m_nodeIndex = 0;

	//forklifts
	Engine1::ref<Engine1::VertexArray> m_forkliftVA;
	Engine1::ref<Engine1::Texture2D> m_forkliftTex;
	std::vector<Forklift> m_forklifts;

	int m_forkliftIndex = 0;

	//scale
	Engine1::ref<Engine1::VertexArray> m_scaleVA;
	Engine1::ref<Engine1::Texture2D> m_scaleTex;
	std::unique_ptr<Scale> m_scale;

	bool m_showScale = false;
	

	//camera
	Engine1::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	glm::vec2 m_mouseScenePos;
	glm::vec2 m_mouseScreenPos;
	float m_cameraRotation = 0.0f;

	float m_cameraMoveSpeed = 2.0f;
	float m_cameraRotationSpeed = 180.0f;

	bool show = true;

	//Networking
	UDPServer m_server;
	//UDPClient m_client;
	//int i = 0;


	std::vector<long long> SpeedTest;
	int count = 0;


	//timestamp stuff
	std::string m_fileName;

public:
	Layer1() : Layer("Layer1"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f) {

		srand(time(nullptr));

		std::time_t result = std::time(nullptr);
		m_fileName = "Log_";
		m_fileName += std::asctime(std::localtime(&result));
	
		m_groundPlanWallsTex = Engine1::Texture2D::create("assets/textures/grid.png");	//Engine1::Texture2D::create("assets/textures/pudorys-zdi.png");
		m_groundPlanTex = Engine1::Texture2D::create("assets/textures/pudorys.png");
		m_anchorTex = Engine1::Texture2D::create("assets/textures/anchor.png");
		m_nodeTex = Engine1::Texture2D::create("assets/textures/node.png");
		m_scaleTex = Engine1::Texture2D::create("assets/textures/meritko.png");
		m_forkliftTex = Engine1::Texture2D::create("assets/textures/forklift.png");
		
		std::thread t1(&UDPServer::onUpdate, &m_server);
		t1.detach();

		/*std::thread t2(&UDPClient::send, &m_client, "yes");
		t2.detach();*/

		//m_scale->setPosition({ -1.3f, 0.8f, 0.0f });

		/*m_vertexArray.reset(Engine1::VertexArray::create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Engine1::ref<Engine1::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Engine1::VertexBuffer::create(vertices, sizeof(vertices)));

		Engine1::BufferLayout layout = {
			{ Engine1::ShaderDataType::Float3, "a_position" },
			{ Engine1::ShaderDataType::Float4, "a_color" }
		};

		vertexBuffer->setLayout(layout);
		m_vertexArray->addVertexBuffer(vertexBuffer);		//volat az po setLayout



		uint32_t indices[3] = { 0, 1, 2 };
		Engine1::ref<Engine1::IndexBuffer> indexBuffer;
		indexBuffer.reset(Engine1::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->setIndexBuffer(indexBuffer);*/

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	na render noveho objektu je potreba nejdrive vytvoreni VertexArray, pak definice vertexu			   //
		//	pote novy VertexBuffer, pak layout tohoto bufferu a setnuti tohoto layoutu pro tento buffer			   //
		//	dalsi pridame VertexBuffer do VertexArray, zbyvaji indexy -> definice indexu, pak novy IndexBuffer	   //
		//	ten setneme pro VertexArray, zbyva uz jen vykreslit - nabindovat prislusny shader a vertex array	   //
		//	nakonec glDrawElements, kde pocet indexu je VertexArray->getIndexBuffer()->getCount()				   //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		m_squareVA.reset(Engine1::VertexArray::create());

		float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Engine1::ref<Engine1::VertexBuffer> squareVB;
		squareVB.reset(Engine1::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		Engine1::BufferLayout squareVBLayout = {
			{ Engine1::ShaderDataType::Float3, "a_position" },
			{ Engine1::ShaderDataType::Float2, "a_texCoord" }
		};
		squareVB->setLayout(squareVBLayout);
		m_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine1::ref<Engine1::IndexBuffer> squareIB;
		squareIB.reset(Engine1::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_squareVA->setIndexBuffer(squareIB);


		//background texture/////////////////////////////////////////////////////////////////////////////////////////////////
		m_backgroundVA.reset(Engine1::VertexArray::create());

		float groundW = m_groundPlanWallsTex->getWidth();
		float groundH = m_groundPlanWallsTex->getHeight();

		groundW = groundW / 1280 * 1.6f;
		groundH = groundH / 720 * 0.9f;

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

		Engine1::ref<Engine1::VertexBuffer> backgroundVB;
		backgroundVB.reset(Engine1::VertexBuffer::create(backgroundVertices, sizeof(backgroundVertices)));
		backgroundVB->setLayout(backgroundVBLayout);
		m_backgroundVA->addVertexBuffer(backgroundVB);

		uint32_t backgroundIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine1::ref<Engine1::IndexBuffer> backgroundIB;
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

		Engine1::ref<Engine1::VertexBuffer> anchorVB;
		anchorVB.reset(Engine1::VertexBuffer::create(anchorVertices, sizeof(anchorVertices)));
		anchorVB->setLayout(anchorVBLayout);
		m_anchorVA->addVertexBuffer(anchorVB);

		uint32_t anchorIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine1::ref<Engine1::IndexBuffer> anchorIB;
		anchorIB.reset(Engine1::IndexBuffer::create(anchorIndices, sizeof(anchorIndices) / sizeof(uint32_t)));
		m_anchorVA->setIndexBuffer(anchorIB);

		//node//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_nodeVA.reset(Engine1::VertexArray::create());
		float nodeVertices[] = {
			-0.1f, -0.1f, 0.0f, 0.0f, 0.0f,	//left bot
			 0.1f, -0.1f, 0.0f, 1.0f, 0.0f,	//right bot
			 0.1f,  0.1f, 0.0f, 1.0f, 1.0f,	//right top
			-0.1f,  0.1f, 0.0f, 0.0f, 1.0f	//left top
		};

		Engine1::BufferLayout nodeVBLayout = {
			{ Engine1::ShaderDataType::Float3, "a_position" },
			{ Engine1::ShaderDataType::Float2, "a_texPos" }
		};

		Engine1::ref<Engine1::VertexBuffer> nodeVB;
		nodeVB.reset(Engine1::VertexBuffer::create(nodeVertices, sizeof(nodeVertices)));
		nodeVB->setLayout(nodeVBLayout);
		m_nodeVA->addVertexBuffer(nodeVB);

		uint32_t nodeIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine1::ref<Engine1::IndexBuffer> nodeIB;
		nodeIB.reset(Engine1::IndexBuffer::create(nodeIndices, sizeof(nodeIndices) / sizeof(uint32_t)));
		m_nodeVA->setIndexBuffer(nodeIB);


		//forklift//////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_forkliftVA.reset(Engine1::VertexArray::create());
		float forkliftVertices[] = {
			-0.1f, -0.1f, 0.0f, 0.0f, 0.0f,	//left bot
			 0.1f, -0.1f, 0.0f, 1.0f, 0.0f,	//right bot
			 0.1f,  0.1f, 0.0f, 1.0f, 1.0f,	//right top
			-0.1f,  0.1f, 0.0f, 0.0f, 1.0f	//left top
		};

		Engine1::BufferLayout forkliftVBLayout = {
			{ Engine1::ShaderDataType::Float3, "a_position" },
			{ Engine1::ShaderDataType::Float2, "a_texPos" }
		};

		Engine1::ref<Engine1::VertexBuffer> forkliftVB;
		forkliftVB.reset(Engine1::VertexBuffer::create(forkliftVertices, sizeof(forkliftVertices)));
		forkliftVB->setLayout(forkliftVBLayout);
		m_forkliftVA->addVertexBuffer(forkliftVB);

		uint32_t forkliftIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine1::ref<Engine1::IndexBuffer> forkliftIB;
		forkliftIB.reset(Engine1::IndexBuffer::create(forkliftIndices, sizeof(forkliftIndices) / sizeof(uint32_t)));
		m_forkliftVA->setIndexBuffer(forkliftIB);


		//scale/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_scaleVA.reset(Engine1::VertexArray::create());

		float scaleW = m_scaleTex->getWidth();
		float scaleH = m_scaleTex->getHeight();

		m_scale = std::make_unique<Scale>(Scale(scaleW, scaleH, { 480.0f, 80.0f, 0.0f }));

		scaleW = scaleW / 1280 * 1.6f;
		scaleH = scaleH / 720 * 0.9f;


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

		Engine1::ref<Engine1::VertexBuffer> scaleVB;
		scaleVB.reset(Engine1::VertexBuffer::create(scaleVertices, sizeof(scaleVertices)));
		scaleVB->setLayout(scaleVBLayout);
		m_scaleVA->addVertexBuffer(scaleVB);

		uint32_t scaleIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine1::ref<Engine1::IndexBuffer> scaleIB;
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

		//tutorial/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			void main()
			{
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_color;

			void main()
			{
				color = vec4(u_color, 1.0);
			}
		)";

		m_flatColorShader.reset(Engine1::Shader::create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

	

		m_texture = Engine1::Texture2D::create("assets/textures/pudorys-zdi.png");

		std::dynamic_pointer_cast<Engine1::OpenGLShader>(m_textureShader)->bind();
		std::dynamic_pointer_cast<Engine1::OpenGLShader>(m_textureShader)->uploadUniform1i("u_texture", 0);*/
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



		m_shaderLibrary.load("assets/shaders/Texture.glsl");
		

	}

	void onUpdate(Engine1::Timestep ts) override {


		Engine1::RenderCommand::setClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Engine1::RenderCommand::clear();

		m_camera.setPosition(m_cameraPosition);
		m_camera.setRotation(m_cameraRotation);


		//recalculation of mouse scene position based on camera position and zoom////////////////////////////////////
		//TODO: replace 1280, 720 with widnows dimensions
		//TODO: put this to a function
		auto [x, y] = Engine1::Input::getMousePosition();

		x = x * 2 * m_camera.getRight() / 1280 - m_camera.getRight();
		float tmp = m_camera.getRight() / m_camera.getCurrentZoom();
		x = (x + tmp) * 1280 / (tmp * 2);

		y = y * 2 * m_camera.getTop() / 720 - m_camera.getTop();
		tmp = m_camera.getTop() / m_camera.getCurrentZoom();
		y = (y + tmp) * 720 / (tmp * 2);



		//denormalize camera/////////////////////////////////////////////////////////////////////
		glm::vec3 camPos = m_camera.getPosition();
		camPos.x *= 1280 / (m_camera.getRight() * 2);
		camPos.y *= -720 / (m_camera.getTop() * 2);

		m_mouseScenePos = { x + camPos.x, y + camPos.y };

		//screen position from scene position//////////////////////////////////////////////////////////////
		//TODO: replace hard values
		glm::vec2 tmpPos = m_mouseScenePos;

		tmpPos.x -= camPos.x;
		tmpPos.x = m_camera.getRight() * m_camera.getCurrentZoom() * 2 * tmpPos.x / 1280;
		tmp = m_camera.getRight() * m_camera.getCurrentZoom() * m_camera.getCurrentZoom();
		tmpPos.x = (tmpPos.x + tmp) * 1280 / (2 * tmp) - 640 / m_camera.getCurrentZoom();

		tmpPos.y -= camPos.y;
		tmpPos.y = m_camera.getTop() * m_camera.getCurrentZoom() * 2 * tmpPos.y / 720;
		tmp = m_camera.getTop() * m_camera.getCurrentZoom() * m_camera.getCurrentZoom();
		tmpPos.y = (tmpPos.y + tmp) * 720 / (2 * tmp) - 360 / m_camera.getCurrentZoom();
		

		m_mouseScreenPos = { tmpPos.x, tmpPos.y };

		//glm::vec3 tmploc;

		//tmploc = localizeNode();

		Engine1::Renderer::beginScene(m_camera);

		/*glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		std::dynamic_pointer_cast<Engine1::OpenGLShader>(m_flatColorShader)->bind();
		std::dynamic_pointer_cast<Engine1::OpenGLShader>(m_flatColorShader)->uploadUniform3f("u_color", m_squareColor);

		glm::mat4 transform;
		glm::vec3 pos;
		for (int j = 0; j < 20; ++j) {
			for (int i = 0; i < 20; ++i) {
				pos = { i * 0.11f, j * 0.11f, 0.0f };
				transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				
				Engine1::Renderer::submit(m_flatColorShader, m_squareVA, transform);		//blue square
			}
		}

		m_texture->bind();
		Engine1::Renderer::submit(m_textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));*/
			
		//Engine1::Renderer::submit(m_shader, m_vertexArray);		//colored triangle

		auto textureShader = m_shaderLibrary.get("Texture");

		//background
		glm::vec3 pos2(0.0f, 0.0f, 0.0f);
		glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), pos2);
		m_groundPlanWallsTex->bind();
		std::dynamic_pointer_cast<Engine1::OpenGLShader>(textureShader)->uploadUniform1f("u_texture", 0);	//weird on intel gpu
		Engine1::Renderer::submit(textureShader, m_backgroundVA, transform2);

		if (m_showFurniture) {
			m_groundPlanTex->bind();
			std::dynamic_pointer_cast<Engine1::OpenGLShader>(textureShader)->uploadUniform1f("u_texture", 0);
			Engine1::Renderer::submit(textureShader, m_backgroundVA, transform2);
		}

		//anchors
		for (auto& anchor : m_anchors) {
			anchor.setScale(glm::scale(glm::mat4(1.0f), glm::vec3(0.33f)));
			glm::mat4 anchorTransform = glm::translate(glm::mat4(1.0f), anchor.getPosition()) * anchor.getScale();
			m_anchorTex->bind();
			std::dynamic_pointer_cast<Engine1::OpenGLShader>(textureShader)->uploadUniform1f("u_texture", 0);
			Engine1::Renderer::submit(textureShader, m_anchorVA, anchorTransform);
		}

		//network info about nodes
		/*if (m_server.getState()) {
			int anchorID, nodeID, nodex, nodey;
			int nodeIndex = -1;
			std::string msg = m_server.getBuffer();
			std::cout << "message: " << msg << std::endl;

			std::istringstream iss(msg);


			iss >> anchorID >> nodeID >> nodex >> nodey;
			anchorID -= 128;

			for (int i = 0; i < m_nodes.size(); ++i) {
				if (m_nodes[i].getID() == nodeID) {
					nodeIndex = i;
					break;
				}
			}

			if (nodeIndex != -1)
				m_nodes[nodeIndex].setPosition({ nodex, nodey, 0.0f });

			glm::vec3 nodePos;
			if (m_anchors.size() >= 3 && nodeIndex != -1) {
				//nodePos = localizeNode(m_anchors[0], m_anchors[1], m_anchors[2], m_nodes[nodeIndex]);
			}

			if(nodeIndex != -1)
				m_nodes[nodeIndex].setPosition(nodePos);

			m_server.setState(false);
		}*/

		//COM port networking
		if (!m_nodes.empty() && m_server.getState()) {
			glm::vec3 p = m_server.getDists();
			p.x = (p.x / 1000) * m_scale->getMeter();
			p.y = (p.y / 1000) * m_scale->getMeter();
			p.z = (p.z / 1000) * m_scale->getMeter();

			glm::vec3 nposition = localizeNode(m_anchors[0], m_anchors[1], m_anchors[2], p.x, p.y, p.z);

			std::ofstream file;

			//timestamp
			std::time_t result = std::time(nullptr);

			file.open(m_fileName, std::ios::in || std::ios::app);
			file << "[" << std::asctime(std::localtime(&result)) << "]  x: " << nposition.x << " y: " << nposition.y;

			m_nodes[0].setPosition({ nposition.x, nposition.y, 0 });
			m_server.setState(false);
		}

		//nodes
		for (auto& node : m_nodes) {
			node.setScale(glm::scale(glm::mat4(1.0f), glm::vec3(0.33f)));
			glm::mat4 nodeTransform = glm::translate(glm::mat4(1.0f), node.getPosition()) * node.getScale();
			m_nodeTex->bind();
			std::dynamic_pointer_cast<Engine1::OpenGLShader>(textureShader)->uploadUniform1f("u_texture", 0);
			Engine1::Renderer::submit(textureShader, m_nodeVA, nodeTransform);
		}

		//forklifts
		for (auto& forklift : m_forklifts) {
			forklift.setScale(glm::scale(glm::mat4(1.0f), glm::vec3(0.33f)));
			glm::mat4 forkliftTransform = glm::translate(glm::mat4(1.0f), forklift.getPosition()) *	forklift.getScale();
			m_forkliftTex->bind();
			std::dynamic_pointer_cast<Engine1::OpenGLShader>(textureShader)->uploadUniform1f("u_texture", 0);
			Engine1::Renderer::submit(textureShader, m_forkliftVA, forkliftTransform);
		}

		//scale	
		if (m_showScale) {	
			m_scale->setScale(glm::vec3(m_scale->getCurrentWidth() / m_scale->getWidth(), 1.0f, 1.0f));
			glm::mat4 scaleTransform = glm::translate(glm::mat4(1.0f), m_scale->getPosition()) * m_scale->getScale();
			m_scaleTex->bind();
			std::dynamic_pointer_cast<Engine1::OpenGLShader>(textureShader)->uploadUniform1f("u_texture", 0);
			Engine1::Renderer::submit(textureShader, m_scaleVA, scaleTransform);
		}

		Engine1::Renderer::endScene();

		
		
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void onImGuiRender() override {

		static float f1 = m_scale->getWidth();
		static float f2 = m_scale->getScenePosition().x;
		static glm::vec2 pos;
		static glm::vec2 origin;
		static float anchorXpos = 0.0f;
		static float anchorYpos = 0.0f;
		static float north, south, west, east;
		static glm::vec4 anchorWalls;
		unsigned char mouseCol[4];
		auto [xx, yy] = Engine1::Input::getMousePosition();

		ImGui::Begin("Debug");
		//ImGui::ColorPicker3("Square Color", glm::value_ptr(m_squareColor));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Number of anchors: %d", m_anchors.size());
		ImGui::Text("Number of nodes: %d", m_nodes.size());
		ImGui::Text("Mouse scene position: %f %f", m_mouseScenePos.x, m_mouseScenePos.y);
		ImGui::Text("Mouse screen position: %f %f", xx, yy);
		ImGui::Text("Mouse screen pos from scene pos: %.3f %.3f", m_mouseScreenPos.x, m_mouseScreenPos.y);
		ImGui::Text("Camera position: %f %f", (float)m_camera.getPosition().x * 1280 / 3.2, (float)m_camera.getPosition().y * -720 / 1.8);
		ImGui::Text("Camera zoom: %f", m_camera.getCurrentZoom());
		glReadPixels(xx, 720 - yy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, mouseCol);		//TODO: change 720
		ImGui::Text("Color on mouse pos: R:%u G:%u B:%u A:%u", mouseCol[0], mouseCol[1], mouseCol[2], mouseCol[3]);
		if (ImGui::Checkbox("Show Scale", &m_showScale)) {}
		if (ImGui::Checkbox("Show Furniture", &m_showFurniture)) {}
		ImGui::Text("Scale width: %f  height: %f", m_scale->getWidth(), m_scale->getHeight());
		ImGui::Text("Meter in pixels: %f", m_scale->getMeter());
		ImGui::End();

		//saving
		//TODO: remember last save file name
		//TODO: warning about overwriting
		//TODO: shortcuts
		//FIX: load/save bugs, loads one more anchor, crash when closing open dialog without selecting
		if (ImGui::BeginMainMenuBar()){
			if (ImGui::BeginMenu("File")){
				if (ImGui::MenuItem("Save", "CTRL+S")) {
					std::ofstream saveFile;
					saveFile.open("saves\\save.txt");

					saveFile << m_scale->getCurrentWidth() << '\n';

					for (auto anchor : m_anchors) {
						saveFile << anchor.getScenePosition().x << " " << anchor.getScenePosition().y << " " << anchor.getID() << "\n";
					}


					saveFile.close();

				}
				if (ImGui::MenuItem("Save as", "CTRL+S")) {

					//Windows ONLY, needs to be abstracted perhaps or just check OS, then ifs

					OPENFILENAME ofn;       // common dialog box structure
					char szFile[260];       // buffer for file name

					// Initialize OPENFILENAME
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.lpstrFile = (LPWSTR)szFile;
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(szFile);
					LPCWSTR a = L".txt";
					ofn.lpstrFilter = a;
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = nullptr;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = nullptr;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					GetSaveFileName(&ofn);

					std::ofstream saveFile;
					std::stringstream iss;
					std::string str;

					for (int i = 0; ofn.lpstrFile[i] != '\0'; ++i) {
							str += ofn.lpstrFile[i];
					}


					if (!(str[str.size() - 1] == 't' && str[str.size() - 2] == 'x' && str[str.size() - 3] == 't' && str[str.size() - 4] == '.')) {
						str += ".txt";
					}


					E1_INFO("Saving File {0}", str.c_str());
					saveFile.open(str.c_str(), std::ios::out);

					if (saveFile.is_open()) {
						saveFile << m_scale->getCurrentWidth() << '\n';

						for (auto anchor : m_anchors) {
							saveFile << anchor.getScenePosition().x << " " << anchor.getScenePosition().y << " " << anchor.getID() << "\n";
						}
					}
					else
						E1_ERROR("Failed to save file");

				}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {

					//Windows ONLY, needs to be abstracted perhaps or just check OS, then ifs
					OPENFILENAME ofn;       // common dialog box structure
					char szFile[260];       // buffer for file name
					HANDLE hf;              // file handle

					// Initialize OPENFILENAME
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.lpstrFile = (LPWSTR)szFile;
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(szFile);
					LPCWSTR a = L".txt";
					ofn.lpstrFilter = a;
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = nullptr;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = nullptr;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					// Display the Open dialog box. 
					if (GetOpenFileName(&ofn) == TRUE)
						hf = CreateFile(ofn.lpstrFile,
							GENERIC_READ,
							0,
							(LPSECURITY_ATTRIBUTES)nullptr,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							(HANDLE)nullptr);


					std::ifstream openFile;
					std::stringstream iss;
					std::string str;

					for (int i = 0; i < 259; ++i) {
						if(i == 0 || (i + 1) % 2)
							str += szFile[i];
					}


					float xp, yp;
					int id;
					int scalex;

					CloseHandle(hf);
					E1_INFO("Opening File {0}", str.c_str());
					openFile.open(str.c_str(), std::ios::in);

					if (openFile.is_open()) {

						m_anchors.clear();
						iss << openFile.rdbuf();
						openFile.close();

						iss >> scalex;
						m_scale->setWidth(scalex);

						while (!iss.eof()) {

							iss >> xp >> yp >> id;
							addAnchor({ xp, yp }, id);

						}
					}
					else
						E1_ERROR("Failed to open file");

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		

		//right click
		static bool found = false;
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseClicked(1)) {
			pos = m_mouseScenePos;

			//right click on anchor
			float distance = 0;
			for (unsigned int i = 0; i < m_anchors.size(); ++i) {
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
		
		if (ImGui::IsMouseClicked(0)) {
			origin = m_mouseScenePos;
			//normalize
			origin.x = origin.x * 2 * m_camera.getRight() / 1280 - m_camera.getRight();
			origin.y = origin.y * 2 * m_camera.getTop() / 720 - m_camera.getTop();
		}

		//mouse hold
		static bool found2 = false;
		static bool found3 = false;
		static bool dragging = false;
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseDragging(0)) {
			float distance = 0;
			pos = m_mouseScenePos;


			//the position of the scale
			//TODO: scale not locked to mouse when dragging
			if (m_showScale && m_mouseScenePos.x >= m_scale->getScenePosition().x - m_scale->getWidth() / 2 && m_mouseScenePos.x <= m_scale->getScenePosition().x + m_scale->getWidth() / 2
				&& m_mouseScenePos.y >= m_scale->getScenePosition().y - m_scale->getHeight() / 2 && m_mouseScenePos.y <= m_scale->getScenePosition().y + m_scale->getHeight() / 2) {

				m_scale->setPosition({ m_mouseScenePos.x, m_mouseScenePos.y, 0.0f });
			}
			else {
				//anchor moving
				if (!dragging && !found2 && !found3) {
					for (unsigned int i = 0; i < m_anchors.size(); ++i) {
						glm::vec2 anchPos = m_anchors[i].getScenePosition();
						distance = glm::distance(anchPos, pos);		//can change to fastDistance, but less accurate
						if (distance < m_anchors[i].getRadius()) {
							m_anchorIndex = i;
							found2 = true;
							break;
						}
					}
				}

				//node moving - ONLY FOR TESTING
				if (!dragging && !found2 && !found3) {
					for (unsigned int i = 0; i < m_nodes.size(); ++i) {
						glm::vec2 nodePos = m_nodes[i].getScenePosition();
						distance = glm::distance(nodePos, pos);	
						if (distance < m_nodes[i].getRadius()) {
							m_nodeIndex = i;
							found3 = true;
							break;
						}
					}
				}

				//anchor
				if (found2 && !m_anchors[m_anchorIndex].getLock()) {
					m_anchors[m_anchorIndex].setPosition({ m_mouseScenePos.x, m_mouseScenePos.y, 0.0f });
				}

				//node
				if (found3) {
					m_nodes[m_nodeIndex].setPosition({ m_mouseScenePos.x, m_mouseScenePos.y, 0.0f });

					//send location in packet format - FOR TESTING//////////////////////////////////////
					int arr[4];

					arr[0] = 128;		//anchor id with starting bit 1, anchor id = 128 + x
					arr[1] = m_nodeIndex;			//node id
					arr[2] = (int)m_mouseScenePos.x;			//distance
					arr[3] = (int)m_mouseScenePos.y;			//+distance

					std::ostringstream os;
					for (int i = 0; i < 4; i++) {
						if (i >= 1) {
							os << ' ';
						}
						os << arr[i];
					}

					std::string s(os.str());
					//m_client.send(s);
					////////////////////////////////////////////////////////////////////////////////////
				}

				//screen moving
				if (!found2 && !found3) {
					dragging = true;
					glm::vec2 mousePos = m_mouseScenePos;

					mousePos.x = mousePos.x * 2 * m_camera.getRight() / 1280 - m_camera.getRight();
					mousePos.y = mousePos.y * 2 * m_camera.getTop() / 720 - m_camera.getTop();

					m_cameraPosition.x -= (mousePos.x - origin.x) * 0.3;
					m_cameraPosition.y += (mousePos.y - origin.y) * 0.3;
				}
			}



		}
		else {
			if (ImGui::IsMouseReleased(0)) {
				found2 = false;
				found3 = false;
				dragging = false;
			}
		}


		//double click on anchor, node or scale
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseDoubleClicked(0)) {
			pos = m_mouseScenePos;
			float distance = 0;
			bool anchorClickOpen = false;
			
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
						anchorClickOpen = true;
						m_anchorIndex = i;
						anchorWalls = getAnchorAllWallDistance(m_anchors[m_anchorIndex]);
						break;
					}
				}

				if (!anchorClickOpen) {
					for (int i = 0; i < m_nodes.size(); ++i) {
						glm::vec2 nodePos = m_nodes[i].getScenePosition();
						distance = glm::distance(nodePos, pos);		//can change to fastDistance, but less accurate
						if (distance < m_nodes[i].getRadius()) {
							ImGui::OpenPopup("nodeClick");
							m_nodeIndex = i;
							break;
						}
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
			m_scale->setPosition({ f2, m_scale->getScenePosition().y, 0.0f });

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("anchorClick")) {
			if (ImGui::Checkbox("Invert Lines", &m_invertLines)) {}
			ImGui::Text("Position: %f %f", m_anchors[m_anchorIndex].getPosition().x, m_anchors[m_anchorIndex].getPosition().y);
			ImGui::Text("Scene position: %f %f", m_anchors[m_anchorIndex].getScenePosition().x, m_anchors[m_anchorIndex].getScenePosition().y);
			ImGui::Text("Anchor ID: %d", m_anchors[m_anchorIndex].getID());

			float meter = m_scale->getMeter() / m_camera.getCurrentZoom();

			ImGui::Text("Meters from northern wall: %.3f", anchorWalls.x / meter);
			ImGui::Text("Meters from southern wall: %.3f", anchorWalls.y / meter);
			ImGui::Text("Meters from western wall: %.3f", anchorWalls.z / meter);
			ImGui::Text("Meters from eastern wall: %.3f", anchorWalls.w / meter);

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("nodeClick")) {
			ImGui::Text("Position: %f %f", m_nodes[m_nodeIndex].getPosition().x, m_nodes[m_nodeIndex].getPosition().y);
			ImGui::Text("Scene position: %f %f", m_nodes[m_nodeIndex].getScenePosition().x, m_nodes[m_nodeIndex].getScenePosition().y);
			ImGui::Text("Node ID: %d", m_nodes[m_nodeIndex].getID());

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
		dispatcher.dispatch<Engine1::MouseScrolledEvent>(E1_BIND_EVENT_FN(Layer1::onMouseScrolledEvent));

	}

	bool onKeyPressedEvent(Engine1::KeyPressedEvent& event) {

		if (event.getKeyCode() == E1_KEY_SPACE) {
			addNode(m_mouseScenePos);
		}

		if (event.getKeyCode() == E1_KEY_C) {
			m_cameraPosition = { 0.0f, 0.0f, 0.0f };
			m_cameraRotation = 0.0f;
			m_camera.setZoom(1.0f);
		}

		if (event.getKeyCode() == E1_KEY_F) {
			addForklift(m_mouseScenePos);
		}

		return false;
	}

	bool onMouseScrolledEvent(Engine1::MouseScrolledEvent& event) {

		if (event.getYOffset() == -1) {
			m_camera.zoomOut();
		}

		if (event.getYOffset() == 1) {
			m_camera.zoomIn();
		}

		return false;
	}

	void addAnchor(const glm::vec2& position, int id = -1) {
		Anchor anchor({ position.x, position.y, 0.0f }, id);
		if (id == -1)
			anchor.setID(rand());
		m_anchors.push_back(anchor);
	}

	void addNode(const glm::vec2& position, int id = -1) {
		Node node({ position.x, position.y, 0.0f }, id);
		if (id == -1) {
			id = 0 + count;
			count++;
			node.setID(id);
		}
		m_nodes.push_back(node);
	}

	void addForklift(const glm::vec2& position, int id = -1) {
		Forklift forklift({ position.x, position.y, 0.0f }, id);
		if (id == -1)
			forklift.setID(rand());
		m_forklifts.push_back(forklift);
	}

	//directions: 0 = north, 1 = south, 2 = west, 3 = east
	float getAnchorWallDistance(const Anchor& anchor, int direction = 0) {
		unsigned char pick_col[4];


		//TODO: 720 to window height
		//TODO: change readpixels to be integrated in Engine1
		//TODO: rework this, proper object detection
		//FIX: currently can't spot a wall, when the wall is not on screen
		//checking for node color is perhaps not necessary, because anchors will be added first,
		//then they do not need to check distances anymore

		if (direction == 0) {
			glm::vec2 pos = getScreenPosFromScenePos(anchor.getScenePosition());
			for (int i = 0; i < 1000; ++i) {
				glReadPixels(pos.x, 720 - pos.y + i, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pick_col);
				if (!(pick_col[0] > 225 && pick_col[1] > 225 && pick_col[2] > 225)) {
					return (float)i;
				}
			}
		}
		else if (direction == 1) {
			glm::vec2 pos = getScreenPosFromScenePos(anchor.getScenePosition());
			for (int i = 0; i < 1000; ++i) {
				glReadPixels(pos.x, 720 - pos.y - i, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pick_col);
				if (!(pick_col[0] > 220 && pick_col[1] > 220 && pick_col[2] > 220)) {
					return (float)i;
				}
			}
		}
		else if (direction == 2) {
			glm::vec2 pos = getScreenPosFromScenePos(anchor.getScenePosition());
			for (int i = 0; i < 1000; ++i) {
				glReadPixels(pos.x - i, 720 - pos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pick_col);
				if (!(pick_col[0] > 220 && pick_col[1] > 220 && pick_col[2] > 220)) {
					return (float)i;
				}
			}
		}
		else if (direction == 3) {
			glm::vec2 pos = getScreenPosFromScenePos(anchor.getScenePosition());
			for (int i = 0; i < 1000; ++i) {
				glReadPixels(pos.x + i, 720 - pos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pick_col);
				if (!(pick_col[0] > 220 && pick_col[1] > 220 && pick_col[2] > 220)) {
					return (float)i;
				}
			}
		}
		

		return -1.0f;
	}

	glm::vec4 getAnchorAllWallDistance(const Anchor& anchor) {
		glm::vec4 res;
		auto textureShader = m_shaderLibrary.get("Texture");

		//HACK: render only walls before checking wall distance, causes flicker
		Engine1::Renderer::beginScene(m_camera);
		glm::vec3 pos2(0.0f, 0.0f, 0.0f);
		glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), pos2);
		m_groundPlanWallsTex->bind();
		std::dynamic_pointer_cast<Engine1::OpenGLShader>(textureShader)->uploadUniform1f("u_texture", 0);	//weird on intel gpu
		Engine1::Renderer::submit(textureShader, m_backgroundVA, transform2);
		Engine1::Renderer::endScene();

		res.x = getAnchorWallDistance(anchor, 0);
		res.y = getAnchorWallDistance(anchor, 1);
		res.z = getAnchorWallDistance(anchor, 2);
		res.w = getAnchorWallDistance(anchor, 3);

		return res;
	}

	glm::vec2 getScreenPosFromScenePos(const glm::vec2& scenePos) {
		glm::vec2 tmpPos = scenePos;
		glm::vec2 camPos = m_camera.getPosition();
		camPos.x *= 1280 / (m_camera.getRight() * 2);
		camPos.y *= -720 / (m_camera.getTop() * 2);

		tmpPos.x -= camPos.x;
		tmpPos.x = m_camera.getRight() * m_camera.getCurrentZoom() * 2 * tmpPos.x / 1280;
		float tmp = m_camera.getRight() * m_camera.getCurrentZoom() * m_camera.getCurrentZoom();
		tmpPos.x = (tmpPos.x + tmp) * 1280 / (2 * tmp) - 640 / m_camera.getCurrentZoom();

		tmpPos.y -= camPos.y;
		tmpPos.y = m_camera.getTop() * m_camera.getCurrentZoom() * 2 * tmpPos.y / 720;
		tmp = m_camera.getTop() * m_camera.getCurrentZoom() * m_camera.getCurrentZoom();
		tmpPos.y = (tmpPos.y + tmp) * 720 / (2 * tmp) - 360 / m_camera.getCurrentZoom();

		return tmpPos;
	}
	
	~Layer1() {
		//server sometimes tries to acces Pending msg bool, but it is destroyed sooner than the server -> results in an exception
		m_server.stopThread();
	}

};

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