#pragma once

#include "Engine1/Renderer/Texture.h"

namespace Engine1 {

	class OpenGLTexture2D : public Texture2D {
	private:
		std::string m_path;
		uint32_t m_width, m_height;
		uint32_t m_rendererID;

	public:

		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t getWidth() const override { return m_width; }
		virtual uint32_t getHeight() const override { return m_height; }

		virtual void bind(uint32_t slot = 0) const override;

	};

}