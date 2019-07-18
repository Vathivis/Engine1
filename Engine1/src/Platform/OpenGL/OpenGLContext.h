#pragma once

#include "Engine1/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Engine1 {

	class OpenGLContext : public GraphicsContext {
	private:
		GLFWwindow* m_windowHandle;


	public:

		OpenGLContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swapBuffers() override;


	};





}