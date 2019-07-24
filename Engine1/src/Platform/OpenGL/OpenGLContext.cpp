#include "E1pch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Engine1 {

	void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle) {
		E1_CORE_ASSERT(windowHandle, "windowHandle is NULL")
	}

	void OpenGLContext::init() {
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		E1_CORE_ASSERT(status, "Failed to initialize Glad");

		E1_CORE_INFO("OpenGL Info:");
		E1_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		E1_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		E1_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

	}

	void OpenGLContext::swapBuffers() {
		glfwSwapBuffers(m_windowHandle);
	}

}

