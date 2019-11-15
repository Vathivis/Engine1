#include "E1pch.h"
#include "Engine1/Renderer/GraphicsContext.h"

#include "Engine1/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Engine1 {

	scope<GraphicsContext> GraphicsContext::create(void* window)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

