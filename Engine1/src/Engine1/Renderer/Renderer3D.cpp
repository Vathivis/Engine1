#include "E1pch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine1 {

	struct Renderer3DStorage {
		ref<VertexArray> cubeVertexArray;
		ref<Shader> flatColorShader;
	};

	static Renderer3DStorage* s_data3D;

	void Renderer3D::init() {

		s_data3D = new Renderer3DStorage;

		s_data3D->cubeVertexArray = VertexArray::create();

		float cubeVertices[8 * 3] = {
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f
		};

		ref<VertexBuffer> cubeVB = VertexBuffer::create(cubeVertices, sizeof(cubeVertices));
		cubeVB->setLayout({
			{ ShaderDataType::Float3, "a_position" }
			});
		s_data3D->cubeVertexArray->addVertexBuffer(cubeVB);

		uint32_t cubeIndices[] = { 0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1 };
		ref<IndexBuffer> cubeIB = IndexBuffer::create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
		s_data3D->cubeVertexArray->setIndexBuffer(cubeIB);

		s_data3D->flatColorShader = Shader::create("assets/shaders/FlatColor.glsl");

	}

	void Renderer3D::shutdown() {
		delete s_data3D;
	}

	void Renderer3D::beginScene() {
		s_data3D->flatColorShader->bind();
		s_data3D->flatColorShader->setMat4("u_viewProjection", glm::mat4(1.0f));
	}

	void Renderer3D::endScene() {

	}

	void Renderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color) {
		s_data3D->flatColorShader->bind();
		s_data3D->flatColorShader->setFloat4("u_color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_data3D->flatColorShader->setMat4("u_transform", transform);

		s_data3D->cubeVertexArray->bind();
		RenderCommand::drawIndexed(s_data3D->cubeVertexArray);
	}

}