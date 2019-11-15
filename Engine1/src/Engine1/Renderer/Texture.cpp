#include "E1pch.h"
#include "Engine1/Renderer/Texture.h"

#include "Engine1/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"


namespace Engine1 {

	ref<Texture2D> Texture2D::create(const std::string& path)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(path);
		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	ref<Engine1::Texture2D> Texture2D::create(uint32_t width, uint32_t height) {
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(width, height);
		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
