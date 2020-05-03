#pragma once
#include "glm/glm.hpp"
#include "Plane.h"
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
	}

	~Frustum();

	void SetFrustum(float angle, float ratio, float nearD, float farD);

	void SetCamera(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up, const glm::vec3& right);

	int PointInFrustum(const glm::vec3& point);

	int SphereInFrustum(const glm::vec3& point, float radius);

	int CubeInFrustum(const glm::vec3& pos, const int SizeOffset);

	void DrawLines(const Shader& shader);

public:

	static enum {
		FRUSTUM_OUTSIDE = 0,
		FRUSTUM_INTERSECT,
		FRUSTUM_INSIDE
	};
	
	Plane planes[6];

	glm::vec3 nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
	glm::vec3 farTopLeft, farTopRight, farBottomLeft, farBottomRight;
	glm::vec3 currentPosition;

	float m_nearDistance, m_farDistance;
	float m_nearWidth, m_nearHeight;
	float m_farWidth, m_farHeight;
	float m_angle, m_ratio, m_tang;

	bool m_FirstLoad = true;

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};
private:

	Frustum() {}

	VertexArray* vao = new VertexArray[6];

	Byte3* NearVertex = new Byte3[4];
	Byte3* FarVertex = new Byte3[4];
	Byte3* TopVertex = new Byte3[4];
	Byte3* LeftVertex = new Byte3[4];
	Byte3* RightVertex = new Byte3[4];
	Byte3* BottomVertex = new Byte3[4];

};