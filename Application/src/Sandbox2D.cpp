#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f) {

}

void Sandbox2D::onAttach() {
	m_squareVA = Engine1::VertexArray::create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Engine1::ref<Engine1::VertexBuffer> squareVB;
	squareVB.reset(Engine1::VertexBuffer::create(squareVertices, sizeof(squareVertices)));
	squareVB->setLayout({
		{ Engine1::ShaderDataType::Float3, "a_position" }
	});
	m_squareVA->addVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Engine1::ref<Engine1::IndexBuffer> squareIB;
	squareIB.reset(Engine1::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_squareVA->setIndexBuffer(squareIB);

	m_flatColorShader = Engine1::Shader::create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::onDetach() {

}

void Sandbox2D::onUpdate(Engine1::Timestep ts) {
	//update
	m_cameraController.onUpdate(ts);


	//render
	Engine1::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Engine1::RenderCommand::clear();

	Engine1::Renderer::beginScene(m_cameraController.getCamera());

	std::dynamic_pointer_cast<Engine1::OpenGLShader>(m_flatColorShader)->bind();
	std::dynamic_pointer_cast<Engine1::OpenGLShader>(m_flatColorShader)->uploadUniform4f("u_color", m_squareColor);

	Engine1::Renderer::submit(m_flatColorShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Engine1::Renderer::endScene();


}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorPicker3("Square Color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Engine1::Event& event) {
	m_cameraController.onEvent(event);
}
