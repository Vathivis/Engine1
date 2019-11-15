#include "E1pch.h"
#include "Engine1/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Engine1 {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	scope<RendererAPI> RendererAPI::create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createScope<OpenGLRendererAPI>();
		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
