#include "Camera.h"

//SetCamera Direction
glm::vec3 Camera::m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//Set Mouse initial offset
float Camera::m_lastX = 640;
float Camera::m_lastY = 360;
//Set Yaw, pitch
float Camera::m_yaw = -90.0f;
float Camera::m_pitch = 0.0f;
//First time moving mouse
bool Camera::m_firstMouse = true;

void Camera::processInput(GLFWwindow* window, float deltaTime)
{
	//Close window if Escape is press
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//Unlock mouse when key 'F' is pressed
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		glfwSetCursorPosCallback(window, NULL);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	//Lock mouse back to screen when space bar is pressed
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_firstMouse = true;
		glfwSetCursorPosCallback(window, mouse_callback);
	}
	
	//Key press to move camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_cameraPos += m_cameraFront * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		m_cameraPos -= m_up * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		m_cameraPos += m_up * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_cameraPos -= m_cameraFront * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_up)) * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_up)) * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_cameraSpeed = 11.5f;
	else
		m_cameraSpeed = 5.5f;
}

void Camera::mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	//Set the last position to mouse position if first time mouse moves
	if (m_firstMouse)
	{
		m_lastX = (float)xPos;
		m_lastY = (float)yPos;
		m_firstMouse = false;
	}

	//Set the offset from the last frame position to current
	float xOffset = (float)xPos - m_lastX;
	float yOffset = m_lastY - (float)yPos; //Reversed since y coordinates range from bottom to top

	//Set last position to current
	m_lastX = (float)xPos;
	m_lastY = (float)yPos;

	//Set the sensitivity so mouse movement isn't too fast
	const float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	//Lock pitch to 90
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	//Calculate look direction based on yaw and pitch
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_cameraFront = glm::normalize(direction);
}
