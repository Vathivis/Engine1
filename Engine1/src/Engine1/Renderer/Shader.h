#pragma once

namespace Engine1 {

	class Shader {
	private:
		uint32_t m_rendererID;

	public:

		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void bind() const;
		void unbind() const;


	};

}