#pragma once

#include "RenderCommand.h"

namespace Engine1 {


	class Renderer {
	private:


	public:

		static void beginScene();
		static void endScene();

		static void submit(const std::shared_ptr<VertexArray>& vertexArray);


		static inline RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
	};


}