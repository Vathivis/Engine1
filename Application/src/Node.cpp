#include "Node.h"

#include "glm/gtx/transform2.hpp"
#include "glm/gtc/type_ptr.hpp"

Node::Node(const glm::vec3& position, float aspectRatio, float zoomLevel, int id /* = -1*/) {
	m_scenePosition = position;
	m_id = id;
	m_aspectRatio = aspectRatio;
	m_zoomLevel = zoomLevel;

	m_position.x = -m_aspectRatio * m_zoomLevel + m_aspectRatio * m_zoomLevel * 2 * position.x / 1280;
	m_position.y = m_zoomLevel - m_zoomLevel * 2 * position.y / 720;
	m_position.z = 0;


	m_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	m_radius = 50.0f;
}

void Node::setPosition(const glm::vec3& position) {
	m_scenePosition = position;
	m_position.x = -m_aspectRatio * m_zoomLevel + m_aspectRatio * m_zoomLevel * 2 * position.x / 1280;
	m_position.y = m_zoomLevel - m_zoomLevel * 2 * position.y / 720;
}

void Node::recalculateRadius() {

	const float* source = (const float*)glm::value_ptr(m_scale);
	m_radius = 100 * source[0] / 2;
}

