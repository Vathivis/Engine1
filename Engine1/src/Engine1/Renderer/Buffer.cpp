#include "E1pch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Engine1 {

	VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size) {
		switch (Renderer::getAPI()) {
			case RendererAPI::None:
				E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;

			case RendererAPI::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);

		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t size) {
		switch (Renderer::getAPI()) {
		case RendererAPI::None:
			E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, size);

		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}

