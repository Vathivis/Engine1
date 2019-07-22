#pragma once

#include "glm/glm.hpp"

namespace Engine1 {

	class OrthographicCamera {
	private:
		glm::mat4 m_projectionM;
		glm::mat4 m_viewM;
		glm::mat4 m_viewProjectionM;		//caching

		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		float m_rotation = 0.0f;

	public:

		OrthographicCamera(float left, float right, float bottom, float top);

		//setters
		void setPosition(const glm::vec3& position) { m_position = position; recalculateViewMatrix(); }
		void setRotation(float rotation) { m_rotation = rotation; recalculateViewMatrix(); }

		//getters
		const glm::vec3& getPosition() const { return m_position; }
		float getRotation() const { return m_rotation; }
		const glm::mat4& getProjectionMatrix() const { return m_projectionM; }
		const glm::mat4& getViewMatrix() const { return m_viewM; }
		const glm::mat4& getViewProjectionMatrix() const { return m_viewProjectionM; }

	private:

		//call whenever we set something - setters
		void recalculateViewMatrix();
	};

}