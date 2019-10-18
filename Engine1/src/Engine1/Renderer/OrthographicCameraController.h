#pragma once

#include "Engine1/Renderer/OrthographicCamera.h"
#include "Engine1/Core/Timestep.h"

#include "Engine1/Events/ApplicationEvent.h"
#include "Engine1/Events/MouseEvent.h"

namespace Engine1 {

	class OrthographicCameraController {
	private:
		//zoom and aspect ration need to be initilized before camera
		float m_aspectRatio;
		float m_zoomLevel = 1.0f;
		bool m_rotation;

		OrthographicCamera m_camera;

		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_cameraRotation = 0.0f; //In degrees, in the anti-clockwise direction

		float m_cameraTranslationSpeed = 5.0f;
		float m_cameraRotationSpeed = 180.0f;
	public:

		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

		//getters
		OrthographicCamera& getCamera() { return m_camera; }
		const OrthographicCamera& getCamera() const { return m_camera; }
		inline float getZoomLevel() { return m_zoomLevel; }
		inline float getAspectRatio() { return m_aspectRatio; }

	private:

		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);

	};



}