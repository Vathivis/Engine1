#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Engine1 {


	class Renderer {
	private:
		struct sceneData {
			glm::mat4 viewProjectionMatrix;
		};
		static sceneData* s_sceneData;

	public:

		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);


		static inline RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
	};


}