#pragma once

#include "Engine1/Renderer/RendererAPI.h"

namespace Engine1 {

	class OpenGLRendererAPI : public RendererAPI {
	private:


	public:

		virtual void init() override;

		virtual void clear() override;
		virtual void setClearColor(const glm::vec4& color) override;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void drawIndexed(const ref<VertexArray>& vertexArray) override;
		//virtual void drawLine(const glm::vec2& src, const glm::vec2& dst) override;

	};

}