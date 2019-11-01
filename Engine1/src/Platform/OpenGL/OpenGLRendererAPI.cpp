#include "E1pch.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Engine1 {

	void OpenGLRendererAPI::init() {
		//texture transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::setClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::drawIndexed(const ref<VertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}

	/*void OpenGLRendererAPI::drawLine(const glm::vec2& src, const glm::vec2& dst) {
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glLineWidth(0.2f);
		glColor3f(0.8f, 0.2f, 0.4f);
		glBegin(GL_LINES);
			glVertex3f(src.x, src.y, 0.0f);
			glVertex3f(dst.x, dst.y, 0.0f);
		glEnd();
	}*/

}