#include "E1pch.h"
#include "Renderer.h"

namespace Engine1 {

	Renderer::sceneData* Renderer::s_sceneData = new Renderer::sceneData;

	void Renderer::beginScene(OrthographicCamera& camera)	{
		s_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene() {

	}

	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, /*const Texture& texture,*/ const glm::mat4& transform) {
		shader->bind();
		shader->uploadUniformMat4("u_viewProjection", s_sceneData->viewProjectionMatrix);
		shader->uploadUniformMat4("u_transform", transform);
		//shader->uploadUniform1i("u_texture", 0);
		//texture.bind(0);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

	/*void Renderer::drawLine(const glm::vec2& src, const glm::vec2& dst) {
		RenderCommand::drawLine(src, dst);
	}*/

}

