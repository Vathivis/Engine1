#pragma once

#include "Engine1/Renderer/Shader.h"
#include "glm/glm.hpp"

#include "glad/glad.h"

namespace Engine1 {

	class OpenGLShader : public Shader {
	private:
		uint32_t m_rendererID;
		mutable std::unordered_map<std::string, GLint> m_uniformLocationCache;

	public:

		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		//set uniforms
		void uploadUniform1i(const std::string& name, int value);

		void uploadUniform1f(const std::string& name, float value);
		void uploadUniform2f(const std::string& name, const glm::vec2& value);
		void uploadUniform3f(const std::string& name, const glm::vec3& value);
		void uploadUniform4f(const std::string& name, const glm::vec4& value);

		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:

		GLint getUniformLocation(const std::string& name) const;

	};

}