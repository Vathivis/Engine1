#include "Scale.h"

#include "glm/gtc/type_ptr.hpp"

Scale::Scale(float width, float height, const glm::vec3& position) {
	m_scenePosition = position;
	m_width = m_currentWidth = width;
	m_height = m_currentHeight = height;
	m_meter = width;

	m_position.x = -1.6f + 3.2f * position.x / 1280;
	m_position.y = 0.9f - 1.8f * position.y / 720;
	m_position.z = 0;

	m_scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	m_scaleVec = glm::vec3(1.0f);
}

void Scale::setPosition(const glm::vec3& position) {
	m_scenePosition = position;
	m_position.x = -1.6f + 3.2f * position.x / 1280;
	m_position.y = 0.9f - 1.8f * position.y / 720;
}

void Scale::recalculateSize() {

	const float* source = (const float*)glm::value_ptr(m_scaleMat);

	m_currentWidth = m_width * source[0];
	m_currentHeight = m_height * source[5];

}
