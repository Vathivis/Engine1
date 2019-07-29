#pragma once

#include <glm/glm.hpp>

namespace Engine1 {
	/*
		Shader = how we are rendering
	*/
	class Shader {
	private:
		uint32_t m_rendererID;

	public:

		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void bind() const;
		void unbind() const;

		//set uniforms
		void uploadUniform1i(const std::string& name, int value);
		void uploadUniform1f(const std::string& name, float value);
		void uploadUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void uploadUniform4f(const std::string& name, glm::vec4 values);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	};

}