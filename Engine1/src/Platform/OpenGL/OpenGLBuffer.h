#pragma once

#include "Engine1/Renderer/Buffer.h"

namespace Engine1 {

	class OpenGLVertexBuffer : public VertexBuffer {
	private:
		uint32_t m_rendererID;


	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

	};

	class OpenGLIndexBuffer : public IndexBuffer {
	private:
		uint32_t m_rendererID;
		uint32_t m_count;

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t getCount() const override { return m_count; }

	};

}