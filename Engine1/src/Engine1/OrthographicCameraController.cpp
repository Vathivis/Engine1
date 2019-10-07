#include "E1pch.h"
#include "OrthographicCameraController.h"

#include "Engine1/Input.h"

namespace Engine1 {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) 
		: m_aspectRatio(aspectRatio), m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio* m_zoomLevel, -m_zoomLevel, m_zoomLevel), m_rotation(rotation) {

	}

	void OrthographicCameraController::onUpdate(Timestep ts) {
		//else if to prevent cancelling each other
		if (Input::isKeyPressed(E1_KEY_D))
			m_cameraPosition.x += m_cameraTranslationSpeed * ts;
		else if (Input::isKeyPressed(E1_KEY_A))
			m_cameraPosition.x -= m_cameraTranslationSpeed * ts;

		if (Input::isKeyPressed(E1_KEY_W))
			m_cameraPosition.y += m_cameraTranslationSpeed * ts;
		else if (Input::isKeyPressed(E1_KEY_S))
			m_cameraPosition.y -= m_cameraTranslationSpeed * ts;

		if (m_rotation) {
			if (Input::isKeyPressed(E1_KEY_E))
				m_cameraRotation += m_cameraRotationSpeed * ts;
			else if (Input::isKeyPressed(E1_KEY_Q))
				m_cameraRotation -= m_cameraRotationSpeed * ts;

			m_camera.setRotation(m_cameraRotation);
		}

		m_camera.setPosition(m_cameraPosition);

		m_cameraTranslationSpeed = m_zoomLevel;
	}

	void OrthographicCameraController::onEvent(Event& e) {
		EventDispatcher dispatcher(e);

		dispatcher.dispatch<MouseScrolledEvent>(E1_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(E1_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));

	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) {
		m_zoomLevel -= e.getYOffset() * 0.25f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);


		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e) {
		m_aspectRatio = (float)e.getWidth() / (float)e.getHeight();
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);


		return false;
	}

}

