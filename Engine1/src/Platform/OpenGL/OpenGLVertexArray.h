#pragma once

#include "Engine1/Renderer/VertexArray.h"

namespace Engine1 {

	class OpenGLVertexArray : public VertexArray {
	private:
		std::vector<ref<VertexBuffer>> m_vertexBuffers;
		ref<IndexBuffer> m_indexBuffer;
		uint32_t m_vertexBufferIndex = 0;
		uint32_t m_rendererID;

	public:

		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void addVertexBuffer(const ref<VertexBuffer>& vertexBuffer) override;
		virtual void setIndexBuffer(const ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<ref<VertexBuffer>>& getVertexBuffers() const override { return m_vertexBuffers; }
		virtual const ref<IndexBuffer>& getIndexBuffer() const override { return m_indexBuffer; }

		static VertexBuffer* create();

	};



}