#include "Anchor.h"
#include "glm/gtx/transform2.hpp"
#include "glm/gtc/type_ptr.hpp"

//#include <Engine1.h>

Anchor::Anchor(const glm::vec3& position) {

	m_scenePosition = position;

	m_position.x = -1.6f + 3.2f * position.x / 1280;
	m_position.y = 0.9f - 1.8f * position.y / 720;

	m_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	m_radius = 50.0f;
}

void Anchor::recalculateRadius() {

	const float* source = (const float*)glm::value_ptr(m_scale);

	m_radius = 100 * source[0] / 2;


}

/*void Anchor::castRays() {
	Engine1::Renderer::drawLine({ m_position.x, m_position.y }, { m_position.x + 0.2, m_position.y + 0.2 });
}*/
