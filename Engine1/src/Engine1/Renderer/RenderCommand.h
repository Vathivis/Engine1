#pragma once

#include "RendererAPI.h"

namespace Engine1 {

	class RenderCommand {
	private:
		static RendererAPI* s_rendererAPI;

	public:

		inline static void clear() {
			s_rendererAPI->clear();
		}
		inline static void setClearColor(const glm::vec4& color) {
			s_rendererAPI->setClearColor(color);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
			s_rendererAPI->drawIndexed(vertexArray);
		}

	};

}