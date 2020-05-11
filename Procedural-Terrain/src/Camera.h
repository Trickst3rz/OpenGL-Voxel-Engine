#pragma once
#include "GLErrorManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:

	Camera(const Camera&) = delete;

	static Camera& GetInstance()
	{
		static Camera instance;
		return instance;
	}

	void processInput(GLFWwindow* window, float deltaTime);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	glm::vec3 GetCameraPosition() { return m_cameraPos; }

	void SetCameraPosition(glm::vec3 newPosition) { m_cameraPos = newPosition; }

	glm::vec3 GetCameraFront() { return m_cameraFront; }

	void SetCameraForward(glm::vec3 newCameraForward) { m_cameraFront = newCameraForward; }

	glm::vec3 GetCameraUp() { return m_cameraUp; }

	glm::vec3 GetCameraRight() { return m_cameraRight; }

private:

	Camera() {};

	//Camera Position
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	//Camera Direction
	static glm::vec3 m_cameraFront;
	//Up Axis
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	//Right Axis
	glm::vec3 m_cameraRight = glm::normalize(glm::cross(Camera::m_up, Camera::m_cameraFront));
	//Up Direction
	glm::vec3 m_cameraUp = glm::cross(Camera::m_cameraFront, Camera::m_cameraRight);

	//Camera Speed
	float m_cameraSpeed = 5.5f;
	//Mouse initial offset
	static float m_lastX, m_lastY;
	//Yaw, pitch
	static float m_yaw, m_pitch;
	//First time moving mouse
	static bool m_firstMouse;
};