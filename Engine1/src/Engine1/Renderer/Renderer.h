#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"

namespace Engine1 {


	class Renderer {
	private:
		struct sceneData {
			glm::mat4 viewProjectionMatrix;
		};
		static scope<sceneData> s_sceneData;

	public:

		static void init();

		static void onWindowResize(uint32_t width, uint32_t height);

		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void submit(const ref<Shader>& shader, const ref<VertexArray>& vertexArray, /*const Texture& texture,*/ const glm::mat4& transform = glm::mat4(1.0f));

		static inline RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
	};


}