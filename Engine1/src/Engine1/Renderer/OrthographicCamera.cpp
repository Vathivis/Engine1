#include "E1pch.h"
#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Engine1 {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) 
		: m_projectionM(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewM(1.0f) {
		m_viewProjectionM = m_projectionM * m_viewM;
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
	}

	void OrthographicCamera::zoomIn() {
		if (m_currentZoom <= 0.81)
			return;
		else {
			m_currentZoom -= m_zoomFactor;
			if (m_currentZoom <= 0.81)
				return;
		}

		m_projectionM = glm::ortho(m_left * m_currentZoom, m_right * m_currentZoom, m_bottom * m_currentZoom, m_top * m_currentZoom, -1.0f, 1.0f);
		m_viewProjectionM = m_projectionM * m_viewM;
	}

	void OrthographicCamera::zoomOut() {
		if (m_currentZoom >= 1.5)
			return;
		else {
			m_currentZoom += m_zoomFactor;
			if (m_currentZoom >= 1.5)
				return;
		}

		m_projectionM = glm::ortho(m_left * m_currentZoom, m_right * m_currentZoom, m_bottom * m_currentZoom, m_top * m_currentZoom, -1.0f, 1.0f);
		m_viewProjectionM = m_projectionM * m_viewM;
	}

	void OrthographicCamera::recalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));		//rotate 3rd argument is rotation axis

		//inversion because camera does not really exist, instead we move all objects to the opposite direction
		m_viewM = glm::inverse(transform);
		m_viewProjectionM = m_projectionM * m_viewM;		//order matters
	}


}