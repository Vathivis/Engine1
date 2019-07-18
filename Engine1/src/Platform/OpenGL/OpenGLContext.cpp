#include "E1pch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Engine1 {

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

	}

	void OpenGLContext::swapBuffers() {
		glfwSwapBuffers(m_windowHandle);
	}

}
