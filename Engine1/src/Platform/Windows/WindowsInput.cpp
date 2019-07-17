#include "E1pch.h"
#include "WindowsInput.h"

#include "Engine1/Application.h"

#include "GLFW/glfw3.h"



namespace Engine1 {

	Input* Input::s_Instance = new WindowsInput();		//staticka inicializace



	bool WindowsInput::isKeyPressedImpl(int keycode) {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || GLFW_REPEAT;

	}

	bool WindowsInput::isMouseButtonPressedImpl(int button) {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::getMousePositionImpl() const {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return std::make_pair<float, float>((float)xpos, (float)ypos);
		//return {(float)xpos, (float)ypos}
	}

	float WindowsInput::getMouseXImpl() const {
		auto [x, y] = getMousePositionImpl();
		return x;
	}

	float WindowsInput::getMouseYImpl() const {
		auto [x, y] = getMousePositionImpl();
		return y;
	}



}
