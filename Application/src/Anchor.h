#pragma once

//#include "Engine1/Renderer/OrthographicCamera.h"

#include "glm/glm.hpp"

class Anchor {
private:
	glm::vec3 m_position;
	glm::vec3 m_scenePosition;
	float m_radius;
	glm::mat4 m_scale;

	bool m_locked = false;

public:

	Anchor() = default;
	Anchor(const glm::vec3& position);

	//getters
	inline glm::vec3 getPosition() const { return m_position; }
	inline glm::vec3 getScenePosition() const { return m_scenePosition; }
	inline glm::mat4 getScale() const { return m_scale; }
	inline float getRadius() const { return m_radius; }
	inline bool getLock() const { return m_locked; }

	//setters
	void setPosition(const glm::vec3& position);
	inline void setScale(const glm::mat4& scale) { m_scale = scale; recalculateRadius(); }
	inline void setLock(bool lock) { m_locked = lock; }

	void recalculateRadius();
	//void castRays();
};