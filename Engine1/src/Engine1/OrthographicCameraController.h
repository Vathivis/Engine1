#pragma once

#include "Engine1/Renderer/OrthographicCamera.h"
#include "Core/Timestep.h"

#include "Engine1/Events/ApplicationEvent.h"
#include "Engine1/Events/MouseEvent.h"

namespace Engine1 {

	class OrthographicCameraController {
	private:
		float m_aspectRatio;
		float m_zoomLevel = 1.0f;

		OrthographicCamera m_camera;

	public:

		OrthographicCameraController(float aspectRatio);

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

	private:

		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);

	};



}