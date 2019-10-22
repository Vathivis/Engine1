#include "E1pch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine1 {

	ref<VertexArray> VertexArray::create() {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:
			E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();

		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}


}

