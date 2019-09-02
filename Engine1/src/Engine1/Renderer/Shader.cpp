#include "E1pch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine1 {

	Engine1::Shader* Shader::create(const std::string& filepath) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:
			E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLShader(filepath);

		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

	Shader* Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:
			E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);

		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}


}

