#include "Anchor.h"

#include "glm/gtx/transform2.hpp"
#include "glm/gtc/type_ptr.hpp"



Anchor::Anchor(const glm::vec3& position, float aspectRatio, float zoomLevel, int id /* = -1*/) {

	m_scenePosition = position;
	m_id = id;
	m_aspectRatio = aspectRatio;
	m_zoomLevel = zoomLevel;

	m_position.x = -(m_aspectRatio) + (2 * m_aspectRatio) * position.x / 1280;
	m_position.y = m_zoomLevel - m_zoomLevel * 2 * position.y / 720;
	m_position.z = 0;


	m_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	m_radius = 50.0f;
}

void Anchor::setPosition(const glm::vec3& position) {
	m_scenePosition = position;
	m_position.x = -m_aspectRatio * m_zoomLevel + (2 * m_aspectRatio * m_zoomLevel) * position.x / 1280;
	m_position.y = m_zoomLevel - (2 * m_zoomLevel) * position.y / 720;
}

void Anchor::recalculateRadius() {

	const float* source = (const float*)glm::value_ptr(m_scale);
	m_radius = 100 * source[0] / 2;
}


