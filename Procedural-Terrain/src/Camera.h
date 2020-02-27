#pragma once

#include "GLErrorManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:

	void processInput(GLFWwindow* window);

	glm::vec3 GetCameraPosition() { return m_cameraPos; }

	glm::vec3 GetCameraForward() { return m_cameraForward; }

	glm::vec3 GetCameraUp() { return m_cameraUp; }

private:
	//Camera Position
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	//Camera Direction
	glm::vec3 m_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_cameraDirection = glm::normalize(m_cameraPos - m_cameraTarget);
	//Right Axis
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_cameraRight = glm::normalize(glm::cross(m_up, m_cameraDirection));
	//Forward Axis
	glm::vec3 m_cameraForward = glm::normalize(glm::cross(m_cameraRight, m_up));
	//Up Axis
	glm::vec3 m_cameraUp = glm::cross(m_cameraDirection, m_cameraRight);
	//Camera Speed
	float cameraSpeed = 0.05f;
};