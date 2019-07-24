#pragma once

#include "glm/glm.hpp"

class Anchor {
private:
	glm::vec3 m_position;


public:

	Anchor() = default;
	Anchor(const glm::vec3& position);

	//getters
	inline glm::vec3 getPosition() const { return m_position; }

	//setters
	inline void setPosition(const glm::vec3& position) { m_position = position; }

};