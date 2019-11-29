#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f) {

}

void Sandbox2D::onAttach() {
	E1_PROFILE_FUNCTION();

	m_checkerboardTexture = Engine1::Texture2D::create("assets/textures/checkerboard.png");
}

void Sandbox2D::onDetach() {
	E1_PROFILE_FUNCTION();

}

void Sandbox2D::onUpdate(Engine1::Timestep ts) {

	//update
	m_cameraController.onUpdate(ts);
	

	//render
	{
		E1_PROFILE_SCOPE("Renderer Prep");
		Engine1::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Engine1::RenderCommand::clear();
	}

	{
		E1_PROFILE_SCOPE("Renderer Draw");
		Engine1::Renderer2D::beginScene(m_cameraController.getCamera());

		Engine1::Renderer2D::drawQuad({ 0.0f, 0.5f }, { 1.0f, 1.0f }, m_checkerboardTexture);
		Engine1::Renderer2D::drawQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.7f, 0.3f, 1.0f });
		Engine1::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, { 0.2f, 0.2f, 0.9f, 1.0f });

		Engine1::Renderer2D::endScene();
	}

}

void Sandbox2D::onImGuiRender() {

	E1_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Engine1::Event& event) {
	m_cameraController.onEvent(event);
}
