#pragma once

#include "E1pch.h"
#include "Engine1/Core.h"

namespace Engine1 {

	class Input {
	private:
		static Input* s_Instance;	//povolena pouze jedna instance tridy

	protected:
		Input() = default;

	public:

		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool isKeyPressed(int keycode) { return s_Instance->isKeyPressedImpl(keycode); }
		inline static bool isMouseButtonPressed(int button) { return s_Instance->isMouseButtonPressedImpl(button); }

		inline static std::pair<float, float> getMousePosition() { return s_Instance->getMousePositionImpl(); }
		inline static float getMouseX() { return s_Instance->getMouseXImpl(); }
		inline static float getMouseY() { return s_Instance->getMouseYImpl(); }
		

	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;
		virtual bool isMouseButtonPressedImpl(int button) = 0;

		virtual std::pair<float, float> getMousePositionImpl() const = 0;
		virtual float getMouseXImpl() const = 0;
		virtual float getMouseYImpl() const = 0;
		

	};






}