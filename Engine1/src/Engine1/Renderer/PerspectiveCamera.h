#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

namespace Engine1 {

	class PerspectiveCamera {
	private:

		glm::mat4 m_projectionM;
		glm::mat4 m_viewM;
		glm::mat4 m_viewProjectionM;		//caching

		glm::vec3 m_position = { 0.0f, 0.0f, 3.0f };
		float m_rotation = 0.0f;

		glm::vec3 m_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_cameraDirection = glm::normalize(m_position - m_cameraTarget);

		glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_cameraRight = glm::normalize(glm::cross(m_up, m_cameraDirection));

		glm::vec3 cameraUp = glm::cross(m_cameraDirection, m_cameraRight);

		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	public:




	};


}