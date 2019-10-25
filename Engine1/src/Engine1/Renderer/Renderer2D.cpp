#include "E1pch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine1 {

	struct Renderer2DStorage {
		ref<VertexArray> quadVertexArray;
		ref<Shader> flatColorShader;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::init() {
		s_data = new Renderer2DStorage();

		s_data->quadVertexArray = VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
			{ ShaderDataType::Float3, "a_position" }
		});
		s_data->quadVertexArray->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_data->quadVertexArray->setIndexBuffer(squareIB);

		s_data->flatColorShader = Shader::create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::shutdown() {
		delete s_data;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera) {

		s_data->flatColorShader->bind();
		s_data->flatColorShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix());

	}

	void Renderer2D::endScene() {

	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {

		s_data->flatColorShader->bind();
		s_data->flatColorShader->setFloat4("u_color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_data->flatColorShader->setMat4("u_transform", transform);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);

	}

}

