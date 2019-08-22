#include "E1pch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include "glad/glad.h"

namespace Engine1 {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_path(path) {

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		E1_CORE_ASSERT(data, "Failed to load image");

		//stbi_load return signed int, needs to be converted
		m_width = width;
		m_height = height;

		//!OpenGL 4.5!/////////////
		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, (channels == 3 ? GL_RGB8 : GL_RGBA8), m_width, m_height);

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, (channels == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::bind(uint32_t slot /*= 0*/) const {
		glBindTextureUnit(slot, m_rendererID);
	}

}