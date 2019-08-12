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

		inline static void drawIndexed(const ref<VertexArray>& vertexArray) {
			s_rendererAPI->drawIndexed(vertexArray);
		}

		/*inline static void drawLine(const glm::vec2& src, const glm::vec2& dst) {
			s_rendererAPI->drawLine(src, dst);
		}*/

	};

}