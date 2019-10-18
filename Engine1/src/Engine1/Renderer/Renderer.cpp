#include "E1pch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine1 {

	scope<Renderer::sceneData> Renderer::s_sceneData = createScope<Renderer::sceneData>();

	void Renderer::init() {
		RenderCommand::init();
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
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_viewProjection", s_sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}


}

