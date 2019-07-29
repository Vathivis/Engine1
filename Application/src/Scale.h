#pragma once

#include "glm/glm.hpp"

class Scale {
private:
	glm::vec3 m_position;
	glm::vec3 m_scenePosition;
	glm::mat4 m_scale;		//scale of the scale

	float m_width, m_height;
	float m_tmpW, m_tmpH;		//storage for widt and height, so that recalculation does not forget original width/height

public:

	Scale() = default;
	Scale(float width, float height, const glm::vec3& position);

	//getters
	inline glm::vec3 getPosition() const { return m_position; }
	inline glm::vec3 getScenePosition() const { return m_scenePosition; }
	inline glm::mat4 getScale() const { return m_scale; }
	inline float getWidth() const { return m_width; }
	inline float getHeight() const { return m_height; }

	//setters
	inline void setPosition(const glm::vec3& position) { m_position = position; }
	inline void setScale(const glm::mat4& scale) { m_scale = scale; recalculateSize(); }
	inline void setWidth(float width) { m_width = width; }
	inline void setHeight(float height) { m_height = height; }

	void recalculateSize();

};

