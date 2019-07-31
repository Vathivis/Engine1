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

		//zoom
		float m_zoomFactor = 0.1f;
		float m_currentZoom = 1.0f;
		float m_left, m_right, m_bottom, m_top;

		//float comparison
		float m_epsilon = 0.000001f;

	public:

		OrthographicCamera(float left, float right, float bottom, float top);

		void zoomIn();
		void zoomOut();

		//setters
		void setPosition(const glm::vec3& position) { m_position = position; recalculateViewMatrix(); }
		void setRotation(float rotation) { m_rotation = rotation; recalculateViewMatrix(); }
		void setZoom(float zoom) { m_currentZoom = zoom; recalculateProjectionMatrix(); }

		//getters
		const glm::vec3& getPosition() const { return m_position; }
		float getRotation() const { return m_rotation; }
		const glm::mat4& getProjectionMatrix() const { return m_projectionM; }
		const glm::mat4& getViewMatrix() const { return m_viewM; }
		const glm::mat4& getViewProjectionMatrix() const { return m_viewProjectionM; }
		float getCurrentZoom() const { return m_currentZoom; }
		float getLeft() const { return m_left; }
		float getRight() const { return m_right; }
		float getBottom() const { return m_bottom; }
		float getTop() const { return m_top; }

	private:

		//call whenever we set something - setters
		void recalculateViewMatrix();
		void recalculateProjectionMatrix();
	};

}