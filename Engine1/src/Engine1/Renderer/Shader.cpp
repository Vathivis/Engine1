#include "E1pch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine1 {

	ref<Shader> Shader::create(const std::string& filepath) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:
			E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filepath);

		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

	ref<Shader> Shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:
			E1_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);

		}

		E1_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	void ShaderLibrary::add(const std::string& name, const ref<Shader>& shader) {
		E1_CORE_ASSERT(!exists(name), "Shader already exists");
		m_shaders[name] = shader;
	}

	void ShaderLibrary::add(const ref<Shader>& shader) {
		auto& name = shader->getName();
		add(name, shader);
	}

	ref<Engine1::Shader> ShaderLibrary::load(const std::string& filepath) {
		auto shader = Shader::create(filepath);
		add(shader);
		return shader;
	}

	ref<Engine1::Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath) {
		auto shader = Shader::create(filepath);
		add(name, shader);
		return shader;
	}

	ref<Engine1::Shader> ShaderLibrary::get(const std::string& name) {
		E1_CORE_ASSERT(exists(name), "Shader not found");
		return m_shaders[name];
	}

	bool ShaderLibrary::exists(const std::string& name) const {
		return m_shaders.find(name) != m_shaders.end();
	}

}

