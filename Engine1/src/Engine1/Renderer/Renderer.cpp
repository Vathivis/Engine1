#include "E1pch.h"
#include "Engine1/Renderer/Renderer.h"

#include "Engine1/Renderer/Renderer2D.h"
#include "Engine1/Renderer/Renderer3D.h"

namespace Engine1 {

	scope<Renderer::sceneData> Renderer::s_sceneData = createScope<Renderer::sceneData>();

	void Renderer::init() {
		RenderCommand::init();
		Renderer2D::init();
		Renderer3D::init();
	}

	void Renderer::shutdown() {
		Renderer2D::shutdown();
		Renderer3D::shutdown();
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::setViewport(0, 0, width, height);
	}

	void Renderer::beginScene(OrthographicCamera& camera) {
		s_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene() {

	}

	void Renderer::submit(const ref<Shader>& shader, const ref<VertexArray>& vertexArray, /*const Texture& texture,*/ const glm::mat4& transform) {
		shader->bind();
		shader->setFloat4("u_color", glm::vec4(1.0f));
		shader->setMat4("u_viewProjection", s_sceneData->viewProjectionMatrix);
		shader->setMat4("u_transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}


}

