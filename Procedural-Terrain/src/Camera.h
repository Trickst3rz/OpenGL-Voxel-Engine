#pragma once

#include "GLErrorManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:

	static void processInput(GLFWwindow* window, float deltaTime);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	static glm::vec3 GetCameraPosition() { return m_cameraPos; }

	static glm::vec3 GetCameraFront() { return m_cameraFront; }

	static void SetCameraForward(glm::vec3 newCameraForward) { m_cameraFront = newCameraForward; }

	static glm::vec3 GetCameraUp() { return m_cameraUp; }

private:
	//Camera Position
	static glm::vec3 m_cameraPos;
	//Camera Direction
	static glm::vec3 m_cameraTarget;
	static glm::vec3 m_cameraDirection;
	//Right Axis
	static glm::vec3 m_up;
	static glm::vec3 m_cameraRight;
	//Forward Axis
	static glm::vec3 m_cameraFront;
	//Up Axis
	static glm::vec3 m_cameraUp;
	//Camera Speed

	static float m_cameraSpeed;
	//Mouse initial offset
	static float m_lastX, m_lastY;
	//Yaw, pitch
	static float m_yaw, m_pitch;
	static bool m_firstMouse;

};