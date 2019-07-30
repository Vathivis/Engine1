#pragma once

#include "glm/glm.hpp"

class Node {
private:
	glm::vec3 m_position;
	glm::vec3 m_scenePosition;
	glm::mat4 m_scale;

public:

	Node() = default;
	Node(const glm::vec3& position);

	//getters
	inline glm::vec3 getPosition() const { return m_position; }
	inline glm::vec3 getScenePosition() const { return m_scenePosition; }
	inline glm::mat4 getScale() const { return m_scale; }

	//setters
	void setPosition(const glm::vec3& position);
	inline void setScale(const glm::mat4& scale) { m_scale = scale; }

};

