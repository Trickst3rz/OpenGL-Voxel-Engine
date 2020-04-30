#include "Frustum.h"
#include "Camera.h"

Frustum::~Frustum()
{

}

void Frustum::SetFrustum(float angle, float ratio, float nearD, float farD)
{
	if (m_angle != angle || m_ratio != ratio || m_nearDistance != nearD || m_farDistance != farD)
	{
		//store the frustum information
		m_angle = angle;
		m_ratio = ratio;
		m_nearDistance = nearD;
		m_farDistance = farD;

		//compute width and height of the near and far planes
		m_tang = glm::tan(glm::radians(angle * 0.5f));
		m_nearHeight = nearD * m_tang;
		m_nearWidth = m_nearHeight * ratio;
		m_farHeight = farD * m_tang;
		m_farWidth = m_farHeight * ratio;
	}
}

void Frustum::SetCamera(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up, const glm::vec3& right)
{ //Check to see if position changes on mouse movement
	if (currentPosition != Camera::GetCameraPosition() || m_FirstLoad)
	{
		glm::vec3 nearCentre, farCentre;
		//Compute the centres of the near and far planes
		nearCentre = pos - dir * m_nearDistance;
		farCentre = pos - dir * m_farDistance;

		//Calculate the 4 corners of the frustum on the near plane
		nearTopLeft = nearCentre + up * m_nearHeight - right * m_nearWidth;
		nearTopRight = nearCentre + up * m_nearHeight + right * m_nearWidth;
		nearBottomLeft = nearCentre - up * m_nearHeight - right * m_nearWidth;
		nearBottomRight = nearCentre - up * m_nearHeight + right * m_nearWidth;

		//Calculate the 4 corners of the frustum on the far plane
		farTopLeft = farCentre + up * m_farHeight - right * m_farWidth;
		farTopRight = farCentre + up * m_farHeight + right * m_farWidth;
		farBottomLeft = farCentre - up * m_farHeight - right * m_farWidth;
		farBottomRight = farCentre - up * m_farHeight + right * m_farWidth;

		//Compute the six planes, the function set3Points assumes that the points are given in counter clockwise order
		planes[TOP].Set3Points(nearTopRight, nearTopLeft, farTopLeft);
		planes[BOTTOM].Set3Points(nearBottomLeft, nearBottomRight, farBottomRight);
		planes[LEFT].Set3Points(nearTopLeft, nearBottomLeft, farBottomLeft);
		planes[RIGHT].Set3Points(nearBottomRight, nearTopRight, farBottomRight);
		planes[NEARP].Set3Points(nearTopLeft, nearTopRight, nearBottomRight);
		planes[FARP].Set3Points(farTopRight, farTopLeft, farBottomLeft);

		currentPosition = Camera::GetCameraPosition();
		m_FirstLoad = false;
	}
}

int Frustum::PointInFrustum(const glm::vec3& point)
{
	int result = FRUSTUM_INSIDE;
	for (int i = 0; i < 6; i++)
	{
		if (planes[i].Distance(point) < 0)
			return (FRUSTUM_OUTSIDE);
	}
	return (result);
}

int Frustum::SphereInFrustum(const glm::vec3& point, float radius)
{
	float distance;
	int result = FRUSTUM_INSIDE;

	for (int i = 0; i < 6; i++)
	{
		distance = planes[i].Distance(point);
		if (distance < -radius)
			return (FRUSTUM_OUTSIDE);
		else if (distance < radius)
			return (FRUSTUM_INTERSECT);
	}
	return (result);
}

int Frustum::CubeInFrustum(const glm::vec3& pos, const int SizeOffset)
{
	int result = FRUSTUM_INSIDE;

	for (int i = 0; i < 6; i++)
	{
		//Reset the counters for corners in and out
		int out = 0;
		int in = 0;

		//Pos: Bottom Left Near
		if (planes[i].Distance(pos) < 0)
			out++;
		else
			in++;

		//Point: Bottom Right Near
		if (planes[i].Distance(glm::ivec3(pos.x + SizeOffset, pos.y, pos.z)) < 0)
			out++;
		else
			in++;

		//Point: Top Left Near
		if (planes[i].Distance(glm::ivec3(pos.x, pos.y + SizeOffset, pos.z)) < 0)
			out++;
		else
			in++;

		//Point: Top Right Near
		if (planes[i].Distance(glm::ivec3(pos.x + SizeOffset, pos.y + SizeOffset, pos.z)) < 0)
			out++;
		else
			in++;

		//Point: Bottom Left Far
		if (planes[i].Distance(glm::ivec3(pos.x, pos.y, pos.z - SizeOffset)) < 0)
			out++;
		else
			in++;

		//Point: Bottom Right Far
		if (planes[i].Distance(glm::ivec3(pos.x + SizeOffset, pos.y, pos.z - SizeOffset)) < 0)
			out++;
		else
			in++;

		//Point: Top Left Far
		if (planes[i].Distance(glm::ivec3(pos.x, pos.y + SizeOffset, pos.z - SizeOffset)) < 0)
			out++;
		else
			in++;

		if (planes[i].Distance(glm::ivec3(pos.x + SizeOffset, pos.y + SizeOffset, pos.z - SizeOffset)) < 0)
			out++;
		else
			in++;

		//If all corners are out
		if (!in)
			return (FRUSTUM_OUTSIDE);
		else if (out) //If some corners are out and others are in
			result = FRUSTUM_INTERSECT;
	}
	return (result);
}
