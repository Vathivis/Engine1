#include "Node.h"

#include "glm/gtx/transform2.hpp"

Node::Node(const glm::vec3& position) {
	m_scenePosition = position;

	m_position.x = -1.6f + 3.2f * position.x / 1280;
	m_position.y = 0.9f - 1.8f * position.y / 720;
	m_position.z = 0;

	m_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
}

void Node::setPosition(const glm::vec3& position) {
	m_scenePosition = position;
	m_position.x = -1.6f + 3.2f * position.x / 1280;
	m_position.y = 0.9f - 1.8f * position.y / 720;
}
