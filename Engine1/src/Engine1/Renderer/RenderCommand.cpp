#include "E1pch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Engine1 {

	scope<RendererAPI> RenderCommand::s_rendererAPI = createScope<OpenGLRendererAPI>();

}

