#include "E1pch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

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
		E1_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		E1_CORE_ASSERT(status, "Failed to initialize Glad");

		E1_CORE_INFO("OpenGL Info:");
		E1_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		E1_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		E1_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		#ifdef E1_ENABLE_ASSERTS
			int versionMajor;
			int versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

			E1_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Engine1 requires at least OpenGL version 4.5!");
		#endif

	}

	void OpenGLContext::swapBuffers() {
		E1_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);
	}

}

