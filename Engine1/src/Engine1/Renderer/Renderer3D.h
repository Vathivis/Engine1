#pragma once

#include "glm/glm.hpp"

namespace Engine1 {

	class Renderer3D {
	public:

		static void init();
		static void shutdown();

		static void beginScene();
		static void endScene();

		//primitives
		static void drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);

	};

}