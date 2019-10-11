#pragma once

#include "glm/glm.hpp"


class Node {
private:
	glm::vec3 m_position;
	glm::vec3 m_scenePosition;
	glm::mat4 m_scale;

	float m_aspectRatio;
	float m_zoomLevel;

	float m_radius;		//texture is 100x100
	int m_id;

public:

	Node() = default;
	Node(const glm::vec3& position, float aspectRatio, float zoomLevel, int id = -1);

	//getters
	inline glm::vec3 getPosition() const { return m_position; }
	inline glm::vec3 getScenePosition() const { return m_scenePosition; }
	inline glm::mat4 getScale() const { return m_scale; }
	inline float getRadius() const { return m_radius; }
	inline int getID() const { return m_id; }

	//setters
	void setPosition(const glm::vec3& position);
	inline void setScale(const glm::mat4& scale) { m_scale = scale; recalculateRadius(); }
	inline void setID(int id) { m_id = id; }

	void recalculateRadius();

};

