#include "Sandbox3D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox3D::Sandbox3D() : Layer("Sandbox3D") {

}

void Sandbox3D::onAttach() {

}

void Sandbox3D::onDetach() {

}

void Sandbox3D::onUpdate(Engine1::Timestep ts) {
	//render
	Engine1::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Engine1::RenderCommand::clear();

	Engine1::Renderer3D::beginScene();

	Engine1::Renderer3D::drawCube({ 0.0f, 0.0f, 0.0f }, glm::vec3(0.5f), { 0.4f, 0.7f, 0.3f, 1.0f });


	Engine1::Renderer3D::endScene();
}

void Sandbox3D::onImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorPicker3("Cube Color", glm::value_ptr(m_cubeColor));
	ImGui::End();
}

void Sandbox3D::onEvent(Engine1::Event& event) {

}



