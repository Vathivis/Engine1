#include "E1pch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Engine1/Events/ApplicationEvent.h"
#include "Engine1/Events/MouseEvent.h"
#include "Engine1/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include "glad/glad.h"

namespace Engine1 {

	static uint32_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description) {
		E1_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	scope<Window> Window::create(const WindowProps& props) {
		return createScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		E1_PROFILE_FUNCTION();

		init(props);
	}

	WindowsWindow::~WindowsWindow() {
		E1_PROFILE_FUNCTION();

		shutdown();
	}

	void WindowsWindow::init(const WindowProps& props) {
		E1_PROFILE_FUNCTION();

		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;


		E1_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		

		if (s_GLFWWindowCount == 0)
		{
			E1_CORE_INFO("Initializing GLFW");
			int success = glfwInit();
			E1_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_window = glfwCreateWindow((int)props.width, (int)props.height, m_data.title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		m_context = GraphicsContext::create(m_window);
		m_context->init();

		
		glfwSetWindowUserPointer(m_window, &m_data);
		setVSync(true);

		//setnuti GLFW callbacku
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);		//nastaveni eventu
			E1_CORE_WARN("Window resize: {0}, {1}", width, height);
			data.eventCallback(event);		//event dispatch > zachyceni nekde
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.eventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.eventCallback(event);

		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xpos, (float)ypos);
			data.eventCallback(event);
		});



	}

	void WindowsWindow::shutdown() {
		E1_PROFILE_FUNCTION();

		glfwDestroyWindow(m_window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0) {
			E1_CORE_WARN("Terminating GLFW");
			glfwTerminate();
		}
	}

	void WindowsWindow::onUpdate() {
		E1_PROFILE_FUNCTION();

		glfwPollEvents();
		m_context->swapBuffers();
	}

	void WindowsWindow::setVSync(bool enabled /*= false*/) {
		E1_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_data.VSync = enabled;
	}

	bool WindowsWindow::isVSync() const {
		return m_data.VSync;
	}

}

