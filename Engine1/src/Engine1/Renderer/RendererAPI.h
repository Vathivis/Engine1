#pragma once

#include "VertexArray.h"

#include "glm/glm.hpp"

namespace Engine1 {

	class RendererAPI {
	public:
		enum class API {
			None = 0, OpenGL = 1
		};

	private:
		static API s_API;

	public:

		virtual void clear() = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void drawLine(const glm::vec2& src, const glm::vec2& dst) = 0;

		inline static API getAPI() { return s_API; }
	};

}