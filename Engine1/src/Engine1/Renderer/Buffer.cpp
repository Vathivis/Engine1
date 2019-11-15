#include "E1pch.h"
#include "Engine1/Renderer/Buffer.h"
#include "Engine1/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Engine1 {

	ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
				E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return createRef<OpenGLVertexBuffer>(vertices, size);

		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t size) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:
			E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return createRef<OpenGLIndexBuffer>(indices, size);

		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}

