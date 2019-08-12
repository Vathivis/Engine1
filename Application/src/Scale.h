#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/transform2.hpp"


//TODO: scale should not be a texture, but an opengl drawing to avoid texture disstortion when scaling the scale
class Scale {
private:
	glm::vec3 m_position;
	glm::vec3 m_scenePosition;
	glm::mat4 m_scaleMat;		//scale of the scale
	glm::vec3 m_scaleVec;

	float m_width, m_height;
	float m_currentWidth, m_currentHeight;		//storage for width and height, so that recalculation does not forget original width/height

	float m_meter;

public:

	Scale() = default;
	Scale(float width, float height, const glm::vec3& position);

	//getters
	inline glm::vec3 getPosition() const { return m_position; }
	inline glm::vec3 getScenePosition() const { return m_scenePosition; }
	inline glm::mat4 getScale() const { return m_scaleMat; }
	inline float getWidth() const { return m_width; }
	inline float getCurrentWidth() { return m_currentWidth; }
	inline float getHeight() const { return m_height; }
	inline float getCurrentHeight() { return m_currentHeight; }
	inline float getMeter() const { return m_meter * m_scaleVec.x; }

	//setters
	void setPosition(const glm::vec3& position);
	inline void setWidth(float width) { m_currentWidth = width; }
	inline void setHeight(float height) { m_currentHeight = height; }
	inline void setScale(const glm::vec3& scale) { m_scaleMat = glm::scale(glm::mat4(1.0f), scale); recalculateSize(); m_scaleVec = scale; }

	void recalculateSize();

};

