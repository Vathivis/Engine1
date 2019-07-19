#pragma once

namespace Engine1 {

	enum class RendererAPI {

		None = 0, OpenGL = 1
	};

	class Renderer {
	private:
		static RendererAPI s_rendererAPI;


	public:

		static inline RendererAPI getAPI() { return s_rendererAPI; }


	};


}