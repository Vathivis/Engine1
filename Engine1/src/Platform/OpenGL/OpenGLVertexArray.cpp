#include "E1pch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Engine1 {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Engine1::ShaderDataType::Float:    return GL_FLOAT;
			case Engine1::ShaderDataType::Float2:   return GL_FLOAT;
			case Engine1::ShaderDataType::Float3:   return GL_FLOAT;
			case Engine1::ShaderDataType::Float4:   return GL_FLOAT;
			case Engine1::ShaderDataType::Mat3:     return GL_FLOAT;
			case Engine1::ShaderDataType::Mat4:     return GL_FLOAT;
			case Engine1::ShaderDataType::Int:      return GL_INT;
			case Engine1::ShaderDataType::Int2:     return GL_INT;
			case Engine1::ShaderDataType::Int3:     return GL_INT;
			case Engine1::ShaderDataType::Int4:     return GL_INT;
			case Engine1::ShaderDataType::Bool:     return GL_BOOL;
		}

		E1_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		//OpenGL 4.5+
		glCreateVertexArrays(1, &m_rendererID);

		//OpenGL 3.0+
		/*glGenVertexArrays(1, &m_rendererID);  
		glBindVertexArray(m_rendererID);*/	//??? test it, glcreate = glgen??

	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_rendererID);
	}

	void OpenGLVertexArray::bind() const {
		glBindVertexArray(m_rendererID);
	}

	void OpenGLVertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const ref<VertexBuffer>& vertexBuffer) {
		E1_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout");		//use setLayout first

		glBindVertexArray(m_rendererID);
		vertexBuffer->bind();


		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_vertexBufferIndex);
			glVertexAttribPointer(m_vertexBufferIndex,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)element.offset);
			m_vertexBufferIndex++;
		}

		m_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const ref<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_rendererID);
		indexBuffer->bind();

		m_indexBuffer = indexBuffer;

	}

}

