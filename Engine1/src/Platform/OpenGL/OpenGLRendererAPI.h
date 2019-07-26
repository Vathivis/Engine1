#pragma once

#include "Engine1/Renderer/RendererAPI.h"

namespace Engine1 {

	class OpenGLRendererAPI : public RendererAPI {
	private:


	public:

		virtual void clear() override;
		virtual void setClearColor(const glm::vec4& color) override;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		//virtual void drawLine(const glm::vec2& src, const glm::vec2& dst) override;

	};

}