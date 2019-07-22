#include "E1pch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Engine1 {

	RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;

}

