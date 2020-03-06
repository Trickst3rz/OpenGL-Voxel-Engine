#include "Camera.h"

glm::vec3 Camera::m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//Camera Direction
glm::vec3 Camera::m_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Camera::m_cameraDirection = glm::normalize(Camera::m_cameraPos - Camera::m_cameraTarget);
//Right Axis
glm::vec3 Camera::m_up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Camera::m_cameraRight = glm::normalize(glm::cross(Camera::m_up, Camera::m_cameraDirection));
//Forward Axis
glm::vec3 Camera::m_cameraFront = glm::normalize(glm::cross(Camera::m_cameraRight, Camera::m_up));
//Up Axis
glm::vec3 Camera::m_cameraUp = glm::cross(Camera::m_cameraDirection, Camera::m_cameraRight);
//Camera Speed
float Camera::m_cameraSpeed = 5.5f;

float Camera::m_lastX = 640;
float Camera::m_lastY = 360;
float Camera::m_yaw = -90.0f;
float Camera::m_pitch = 0.0f;
bool Camera::m_firstMouse = true;


void Camera::processInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		glfwSetCursorPosCallback(window, NULL);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_firstMouse = true;
		glfwSetCursorPosCallback(window, mouse_callback);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_cameraPos += m_cameraFront * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		m_cameraPos -= m_cameraUp * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		m_cameraPos += m_cameraUp * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_cameraPos -= m_cameraFront * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * (m_cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_cameraSpeed = 10.5f;
	else
		m_cameraSpeed = 5.5f;
}

void Camera::mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (m_firstMouse)
	{
		m_lastX = xPos;
		m_lastY = yPos;
		m_firstMouse = false;
	}

	float xOffset = xPos - m_lastX;
	float yOffset = m_lastY - yPos; //Reset since y coordinates range from bottom to top
	m_lastX = xPos;
	m_lastY = yPos;

	const float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
	m_cameraFront = glm::normalize(direction);

}
