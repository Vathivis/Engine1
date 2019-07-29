#include "Scale.h"

#include "glm/gtx/transform2.hpp"
#include "glm/gtc/type_ptr.hpp"

Scale::Scale(float width, float height, const glm::vec3& position) {
	m_scenePosition = position;
	m_width = m_tmpW = width;
	m_height = m_tmpH = height;

	m_position.x = -1.6f + 3.2f * position.x / 1280;
	m_position.y = 0.9f - 1.8f * position.y / 720;
	m_position.z = 0;

	m_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
}

void Scale::recalculateSize() {

	const float* source = (const float*)glm::value_ptr(m_scale);

	m_tmpW = m_width * source[0];
	m_tmpH = m_height * source[5];

}
