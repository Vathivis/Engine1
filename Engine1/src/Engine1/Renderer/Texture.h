#pragma once

#include <string>

#include "Engine1/Core.h"

namespace Engine1 {

	/*class Texture {
	private:
		uint32_t m_rendererID;
		std::string m_filePath;
		unsigned char* m_localBuffer;
		int m_width, m_height, m_BPP;

	public:

		Texture() = default;
		Texture(const std::string& path);
		~Texture();

		void bind(unsigned int slot = 0) const;
		void unbind() const;


		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }


	};*/

	class Texture {
	public:

		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture {
	public:

		static ref<Texture2D> create(const std::string& path);
	};

}
