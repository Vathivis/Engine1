#include "E1pch.h"
#include "Engine1/Renderer/Renderer2D.h"

#include "Engine1/Renderer/VertexArray.h"
#include "Engine1/Renderer/Shader.h"
#include "Engine1/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine1 {

	struct Renderer2DStorage {
		ref<VertexArray> quadVertexArray;
		ref<Shader> textureShader;
		ref<Texture2D> whiteTexture;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::init() {
		E1_PROFILE_FUNCTION();

		s_data = new Renderer2DStorage();

		s_data->quadVertexArray = VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		ref<VertexBuffer> squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
		squareVB->setLayout({
			{ ShaderDataType::Float3, "a_position" },
			{ ShaderDataType::Float2, "a_texCoord" }
		});
		s_data->quadVertexArray->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ref<IndexBuffer> squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_data->quadVertexArray->setIndexBuffer(squareIB);

		s_data->whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTexData = 0xffffffff;
		s_data->whiteTexture->setData(&whiteTexData, sizeof(uint32_t));

		s_data->textureShader = Shader::create("assets/shaders/Texture.glsl");
		s_data->textureShader->bind();
		s_data->textureShader->setInt1("u_texture", 0);

	}

	void Renderer2D::shutdown() {
		E1_PROFILE_FUNCTION();

		delete s_data;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera) {
		E1_PROFILE_FUNCTION();

		s_data->textureShader->bind();
		s_data->textureShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix());

	}

	void Renderer2D::endScene() {
		E1_PROFILE_FUNCTION();

	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		E1_PROFILE_FUNCTION();

		s_data->textureShader->setFloat4("u_color", color);
		s_data->whiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_data->textureShader->setMat4("u_transform", transform);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);

	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture) {
		drawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<Texture2D>& texture) {
		E1_PROFILE_FUNCTION();

		s_data->textureShader->setFloat4("u_color", glm::vec4(1.0f));
		texture->bind();


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->textureShader->setMat4("u_transform", transform);


		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);
	}

}

