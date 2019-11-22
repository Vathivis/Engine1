#pragma once

#include "Engine1/Renderer/OrthographicCamera.h"

#include "glm/glm.hpp"

namespace Engine1 {

	class Renderer3D {
	public:

		static void init();
		static void shutdown();

		static void beginScene(const OrthographicCamera& camera);
		static void endScene();

		//primitives
		static void drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);

	};

}