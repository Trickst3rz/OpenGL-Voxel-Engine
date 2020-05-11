#pragma once
#include "glm/glm.hpp"
#include "Plane.h"
#include "Camera.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "TypeDefines.h"

class Frustum {
public:

	Frustum(const Frustum&) = delete;

	static Frustum& GetInstance()
	{
		static Frustum instance;
		return instance;
	};

	~Frustum();

	void SetFrustum(float angle, float ratio, float nearD, float farD);

	void SetCamera(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up);

	int PointInFrustum(const glm::vec3& point);

	int SphereInFrustum(const glm::vec3& point, float radius);

	int CubeInFrustum(const glm::vec3& pos, const int SizeOffset);

	void DrawLines(const Shader& shader);

public:

	enum {
		FRUSTUM_OUTSIDE = 0,
		FRUSTUM_INTERSECT,
		FRUSTUM_INSIDE
	};
	
	Plane planes[6];

	glm::vec3 nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
	glm::vec3 farTopLeft, farTopRight, farBottomLeft, farBottomRight;

	float m_nearDistance, m_farDistance;
	float m_nearWidth, m_nearHeight;
	float m_farWidth, m_farHeight;
	float m_angle, m_ratio;

private:

	Frustum() {}

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};

	Float3 Vertex[4];
};