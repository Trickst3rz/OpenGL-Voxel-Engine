#include "Frustum.h"

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
		m_nearHeight = glm::tan(angle / 2) * m_nearDistance;
		m_nearWidth = m_nearHeight * m_ratio;
		m_farHeight = glm::tan(angle / 2) * m_farDistance;
		m_farWidth = m_farHeight * m_ratio;
	}
}

void Frustum::SetCamera(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up)
{ 
	glm::vec3 nearCentre, farCentre;

	//X axis of camera with given "up" vector and Z dir
	glm::vec3 X = glm::normalize(glm::cross(dir, up));

	//Y axis of camera is the cross product of Z dir and X dir
	glm::vec3 Y = glm::cross(dir, X);

	//Compute the centres of the near and far planes
	nearCentre = pos + (dir * m_nearDistance);
	farCentre = pos + (dir * m_farDistance);

	//Calculate the 4 corners of the frustum on the near plane
	nearTopLeft = nearCentre + (Y * m_nearHeight) - (X * m_nearWidth);
	nearTopRight = nearCentre + (Y * m_nearHeight) + (X * m_nearWidth);
	nearBottomLeft = nearCentre - (Y * m_nearHeight) - (X * m_nearWidth);
	nearBottomRight = nearCentre - (Y * m_nearHeight) + (X * m_nearWidth);

	//Calculate the 4 corners of the frustum on the far plane
	farTopLeft = farCentre + (Y * m_farHeight) - (X * m_farWidth);
	farTopRight = farCentre + (Y * m_farHeight) + (X * m_farWidth);
	farBottomLeft = farCentre - (Y * m_farHeight) - (X * m_farWidth);
	farBottomRight = farCentre - (Y * m_farHeight) + (X * m_farWidth);

	//Compute the six planes, the function set3Points assumes that the points are given in counter clockwise order
	planes[TOP].Set3Points(nearTopRight, nearTopLeft, farTopLeft);
	planes[BOTTOM].Set3Points(nearBottomLeft, nearBottomRight, farBottomRight);
	planes[LEFT].Set3Points(nearTopLeft, nearBottomLeft, farBottomLeft);
	planes[RIGHT].Set3Points(nearBottomRight, nearTopRight, farBottomRight);
	planes[NEARP].Set3Points(nearTopLeft, nearTopRight, nearBottomRight);
	planes[FARP].Set3Points(farTopRight, farTopLeft, farBottomLeft);

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
		if (planes[i].Distance(pos) > 0)
			out++;
		else
			in++;

		////Point: Bottom Right Near
		if (planes[i].Distance(glm::vec3(pos.x + SizeOffset, pos.y, pos.z)) > 0)
			out++;
		else
			in++;

		//Point: Top Left Near
		if (planes[i].Distance(glm::vec3(pos.x, pos.y + SizeOffset, pos.z)) > 0)
			out++;
		else
			in++;

		//Point: Top Right Near
		if (planes[i].Distance(glm::vec3(pos.x + SizeOffset, pos.y + SizeOffset, pos.z)) > 0)
			out++;
		else
			in++;

		//Point: Bottom Left Far
		if (planes[i].Distance(glm::vec3(pos.x, pos.y, pos.z + SizeOffset)) > 0)
			out++;
		else
			in++;

		//Point: Bottom Right Far
		if (planes[i].Distance(glm::vec3(pos.x + SizeOffset, pos.y, pos.z + SizeOffset)) > 0)
			out++;
		else
			in++;

		//Point: Top Left Far
		if (planes[i].Distance(glm::vec3(pos.x, pos.y + SizeOffset, pos.z + SizeOffset)) > 0)
			out++;
		else
			in++;

		//Point: Top Right Far
		if (planes[i].Distance(glm::vec3(pos.x + SizeOffset, pos.y + SizeOffset, pos.z + SizeOffset)) > 0)
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

void Frustum::DrawLines(const Shader& shader)
{
	if (Global::GetInstance().FrustumCamera && !Global::GetInstance().ToggleFrustum)
	{
		//Draw the frustum with Line loops
		VertexArray vao[6];
		VertexBufferLayout layout;
		layout.Push<float>(3);
		
		//Near plane
		Vertex[0] = Float3(nearTopLeft.x, nearTopLeft.y, nearTopLeft.z);
		Vertex[1] = Float3(nearTopRight.x, nearTopRight.y, nearTopRight.z);
		Vertex[2] = Float3(nearBottomRight.x, nearBottomRight.y, nearBottomRight.z);
		Vertex[3] = Float3(nearBottomLeft.x, nearBottomLeft.y, nearBottomLeft.z);

		VertexBuffer nearBuffer(&Vertex, 4 * sizeof * Vertex);
		vao[0].Bind();
		vao[0].AddBuffer(nearBuffer, layout);
		vao[0].Unbind();

		//Far plane
		Vertex[0] = Float3(farTopLeft.x, farTopLeft.y, farTopLeft.z);
		Vertex[1] = Float3(farTopRight.x, farTopRight.y, farTopRight.z);
		Vertex[2] = Float3(farBottomRight.x, farBottomRight.y, farBottomRight.z);
		Vertex[3] = Float3(farBottomLeft.x, farBottomLeft.y, farBottomLeft.z);

		VertexBuffer farBuffer(&Vertex, 4 * sizeof * Vertex);
		vao[1].Bind();
		vao[1].AddBuffer(farBuffer, layout);
		vao[1].Unbind();

		//Top plane 
		Vertex[0] = Float3(nearTopRight.x, nearTopRight.y, nearTopRight.z);
		Vertex[1] = Float3(nearTopLeft.x, nearTopLeft.y, nearTopLeft.z);
		Vertex[2] = Float3(farTopLeft.x, farTopLeft.y, farTopLeft.z);
		Vertex[3] = Float3(farTopRight.x, farTopRight.y, farTopRight.z);

		VertexBuffer TopBuffer(&Vertex, 4 * sizeof * Vertex);
		vao[2].Bind();
		vao[2].AddBuffer(TopBuffer, layout);
		vao[2].Unbind();

		//Bottom plane
		Vertex[0] = Float3(nearBottomLeft.x, nearBottomLeft.y, nearBottomLeft.z);
		Vertex[1] = Float3(nearBottomRight.x, nearBottomRight.y, nearBottomRight.z);
		Vertex[2] = Float3(farBottomRight.x, farBottomRight.y, farBottomRight.z);
		Vertex[3] = Float3(farBottomLeft.x, farBottomLeft.y, farBottomLeft.z);

		VertexBuffer BottomBuffer(&Vertex, 4 * sizeof * Vertex);
		vao[3].Bind();
		vao[3].AddBuffer(BottomBuffer, layout);
		vao[3].Bind();

		//Left plane 
		Vertex[0] = Float3(nearTopLeft.x, nearTopLeft.y, nearTopLeft.z);
		Vertex[1] = Float3(nearBottomLeft.x, nearBottomLeft.y, nearBottomLeft.z);
		Vertex[2] = Float3(farBottomLeft.x, farBottomLeft.y, farBottomLeft.z);
		Vertex[3] = Float3(farTopLeft.x, farTopLeft.y, farTopLeft.z);

		VertexBuffer LeftBuffer(&Vertex, 4 * sizeof * Vertex);
		vao[4].Bind();
		vao[4].AddBuffer(LeftBuffer, layout);
		vao[4].Unbind();

		//Right plane
		Vertex[0] = Float3(nearBottomRight.x, nearBottomRight.y, nearBottomRight.z);
		Vertex[1] = Float3(nearTopRight.x, nearTopRight.y, nearTopRight.z);
		Vertex[2] = Float3(farTopRight.x, farTopRight.y, farTopRight.z);
		Vertex[3] = Float3(farBottomRight.x, farBottomRight.y, farBottomRight.z);

		VertexBuffer RightBuffer(&Vertex, 4 * sizeof * Vertex);
		vao[5].Bind();
		vao[5].AddBuffer(RightBuffer, layout);
		vao[5].Unbind();

		for (int i = 0; i < 6; i++)
		{
			Renderer::DrawLineLoop(vao[i], shader, 4);
		}
	}
}
