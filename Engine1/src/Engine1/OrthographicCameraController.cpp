#include "E1pch.h"
#include "OrthographicCameraController.h"

namespace Engine1 {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio) {

	}

	void OrthographicCameraController::onUpdate(Timestep ts) {
		//else if to prevent cancelling each other
		if (Input::isKeyPressed(E1_KEY_D))
			m_cameraPosition.x += m_cameraMoveSpeed * ts;
		else if (Input::isKeyPressed(E1_KEY_A))
			m_cameraPosition.x -= m_cameraMoveSpeed * ts;

		if (Input::isKeyPressed(E1_KEY_W))
			m_cameraPosition.y += m_cameraMoveSpeed * ts;
		else if (Input::isKeyPressed(E1_KEY_S))
			m_cameraPosition.y -= m_cameraMoveSpeed * ts;


		if (Input::isKeyPressed(E1_KEY_E))
			m_cameraRotation += m_cameraRotationSpeed * ts;
		else if (Input::isKeyPressed(E1_KEY_Q))
			m_cameraRotation -= m_cameraRotationSpeed * ts;
	}

	void OrthographicCameraController::onEvent(Event& e) {

	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) {

	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e) {

	}

}

