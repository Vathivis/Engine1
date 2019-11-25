#include "Sandbox3D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

Sandbox3D::Sandbox3D() : Layer("Sandbox3D"), m_cameraController(1280.0f / 720.0f) {

}

void Sandbox3D::onAttach() {
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void Sandbox3D::onDetach() {

}

void Sandbox3D::onUpdate(Engine1::Timestep ts) {

	//update
	m_cameraController.onUpdate(ts);

	//render
	Engine1::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Engine1::RenderCommand::clear();

	Engine1::Renderer3D::beginScene(m_cameraController.getCamera());

	Engine1::Renderer3D::drawCube({ 0.0f, 0.0f, 0.0f }, glm::vec3(1.0f), { 0.4f, 0.7f, 0.3f, 1.0f });


	Engine1::Renderer3D::endScene();
}

void Sandbox3D::onImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Cube Color", glm::value_ptr(m_cubeColor));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void Sandbox3D::onEvent(Engine1::Event& event) {
	m_cameraController.onEvent(event);
}



