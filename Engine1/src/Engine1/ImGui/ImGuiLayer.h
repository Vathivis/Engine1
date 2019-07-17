#pragma once

#include "Engine1/Layer.h"

#include "Engine1/Events/ApplicationEvent.h"
#include "Engine1/Events/KeyEvent.h"
#include "Engine1/Events/MouseEvent.h"

namespace Engine1 {

	class ENGINE1_API ImGuiLayer : public Layer {
	private:
		float m_time = 0.0f;

	public:

		ImGuiLayer();
		~ImGuiLayer();


		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(Event& event);

	private:
		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool onMouseMovedEvent(MouseMovedEvent& e);
		bool onMouseScrolledEvent(MouseScrolledEvent& e);

		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onKeyReleasedEvent(KeyReleasedEvent& e);
		bool onKeyTypedEvent(KeyTypedEvent& e);

		bool onWindowResizeEvent(WindowResizeEvent& e);

	};

}