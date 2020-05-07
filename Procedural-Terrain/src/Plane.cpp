#include "Plane.h"

Plane::Plane(glm::vec3& v1, glm::vec3& v2, glm::vec3& v3)
{
	Set3Points(v1, v2, v3);
}

Plane::Plane() {}

float Plane::Distance(const glm::vec3& vec)
{
	float check = glm::dot(m_normal, vec);
	return (m_distance + glm::dot(m_normal, vec));
}

void Plane::Set3Points(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
	glm::vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	m_normal = glm::normalize(glm::cross(aux2, aux1));

	m_point = v2;
	m_distance = -(glm::dot(m_normal, m_point));
}

void Plane::SetNormalAndPoint(const glm::vec3& normal, const glm::vec3& point)
{
	m_normal = normal;
	m_normal = glm::normalize(m_normal);
	m_distance = -(glm::dot(m_normal, point));
}

Plane::~Plane()
{

}

