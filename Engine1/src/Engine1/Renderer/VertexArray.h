#pragma once

#include <memory>
#include "Engine1/Renderer/Buffer.h"

namespace Engine1 {

	class VertexArray {
	private:

	public:

		virtual ~VertexArray() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(const ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const ref<IndexBuffer>& vertexBuffer) = 0;

		virtual const std::vector<ref<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const ref<IndexBuffer>& getIndexBuffer() const = 0;

		static VertexArray* create();

	};



}