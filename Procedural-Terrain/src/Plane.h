#pragma once
#include "glm/glm.hpp"

class Plane {
public:
	Plane(glm::vec3& v1, glm::vec3& v2, glm::vec3& v3);

	Plane();

	~Plane();

	float Distance(const glm::vec3& vec);

	void Set3Points(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);

	void SetNormalAndPoint(const glm::vec3& normal, const glm::vec3& point);

private:
	float m_distance;

	glm::vec3 m_normal, m_point;
};