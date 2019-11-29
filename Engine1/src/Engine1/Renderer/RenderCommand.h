#pragma once

#include "Engine1/Renderer/RendererAPI.h"

namespace Engine1 {

	class RenderCommand {
	private:
		static scope<RendererAPI> s_rendererAPI;

	public:

		inline static void init() {
			E1_PROFILE_FUNCTION();
			s_rendererAPI->init();
		}

		inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			s_rendererAPI->setViewport(x, y, width, height);
		}

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