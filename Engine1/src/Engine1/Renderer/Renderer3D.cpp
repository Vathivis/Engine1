#include "E1pch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

namespace Engine1 {

	struct Renderer3DStorage {
		ref<VertexArray> cubeVertexArray;
		ref<Shader> cubeShader;
	};

	static Renderer3DStorage* s_data3D;

	void Renderer3D::init() {
		E1_PROFILE_FUNCTION();

		s_data3D = new Renderer3DStorage;

		s_data3D->cubeVertexArray = VertexArray::create();

		float cubeVertices[8 * 6] = {
			// front
			-1.0, -1.0,  1.0,   1.0, 0.0, 0.0,
			 1.0, -1.0,  1.0,   0.0, 1.0, 0.0,
			 1.0,  1.0,  1.0,   0.0, 0.0, 1.0,
			-1.0,  1.0,  1.0,   1.0, 1.0, 1.0,
			// back
			-1.0, -1.0, -1.0,   1.0, 0.0, 0.0,
			 1.0, -1.0, -1.0,   0.0, 1.0, 0.0,
			 1.0,  1.0, -1.0,   0.0, 0.0, 1.0,
			-1.0,  1.0, -1.0,   1.0, 1.0, 1.0
		};

		ref<VertexBuffer> cubeVB = VertexBuffer::create(cubeVertices, sizeof(cubeVertices));
		cubeVB->setLayout({
			{ ShaderDataType::Float3, "a_position" },
			{ ShaderDataType::Float3, "a_color" }
			});
		s_data3D->cubeVertexArray->addVertexBuffer(cubeVB);

		uint32_t cubeIndices[] = { // front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3
		};
		ref<IndexBuffer> cubeIB = IndexBuffer::create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
		s_data3D->cubeVertexArray->setIndexBuffer(cubeIB);

		s_data3D->cubeShader = Shader::create("assets/shaders/Cube.glsl");

	}

	void Renderer3D::shutdown() {
		delete s_data3D;
	}

	void Renderer3D::beginScene(const OrthographicCamera& camera) {
		E1_PROFILE_FUNCTION();

		s_data3D->cubeShader->bind();
		float angle = glfwGetTime() * 45;  // 45° per second
		glm::vec3 axis_y(0, 1, 0);
		glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -3.5));
		glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 projection = glm::perspective(45.0f, 1.0f * 16/9, 0.1f, 10.0f);
		glm::mat4 result = projection * view * model * anim;
		s_data3D->cubeShader->setMat4("u_viewProjection", result);
	}

	void Renderer3D::endScene() {
		E1_PROFILE_FUNCTION();

	}

	void Renderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color) {
		E1_PROFILE_FUNCTION();

		s_data3D->cubeShader->bind();
		//s_data3D->cubeShader->setFloat4("u_color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_data3D->cubeShader->setMat4("u_transform", transform);

		s_data3D->cubeVertexArray->bind();
		RenderCommand::drawIndexed(s_data3D->cubeVertexArray);
	}



}