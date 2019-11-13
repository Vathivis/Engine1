#pragma once

#include <string>

#include "Engine1/Core/Core.h"

namespace Engine1 {

	class Texture {
	public:

		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void setData(void* data, uint32_t size) = 0;

		virtual void bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture {
	public:

		static ref<Texture2D> create(const std::string& path);
		static ref<Texture2D> create(uint32_t width, uint32_t height);
	};

}
