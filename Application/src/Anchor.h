#pragma once

#include "glm/glm.hpp"

class Anchor {
private:
	glm::vec3 m_position;
	glm::vec3 m_scenePosition;
	float m_radius;
	glm::mat4 m_scale;

public:

	Anchor() = default;
	Anchor(const glm::vec3& position);

	//getters
	inline glm::vec3 getPosition() const { return m_position; }
	inline glm::vec3 getScenePosition() const { return m_scenePosition; }
	inline glm::mat4 getScale() const { return m_scale; }
	inline float getRadius() const { return m_radius; }

	//setters
	inline void setPosition(const glm::vec3& position) { m_position = position; }
	inline void setScale(const glm::mat4& scale) { m_scale = scale; recalculateRadius(); }

	void recalculateRadius();
	//void castRays();
};