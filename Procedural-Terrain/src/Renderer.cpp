#include "Renderer.h"


void Renderer::Clear()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::DrawInstanced(const VertexArray& va, const Shader& shader, const unsigned int& IndicesCount, const int& InstanceCount)
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, IndicesCount, InstanceCount));
	m_DrawCalls++;
}

void Renderer::Draw(const VertexArray& va, const Shader& shader, const int& count)
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
	m_DrawCalls++;
}

short Renderer::m_DrawCalls = 0;

//Make this better later on but will do for now
unsigned short Renderer::IndicesFront[] = {
		//Front 
		0, 1, 2,
		2, 3, 0,
};

unsigned short Renderer::IndicesBack[] = {
		//Back
		4, 5, 6,
		6, 7, 4,
};

unsigned short Renderer::IndicesRight[] = {
		//Right
		8, 9, 10,
		10, 4, 8,
};

unsigned short Renderer::IndicesLeft[] = {
		//Left
		11, 2, 12,
		12, 13, 11,
};

unsigned short Renderer::IndicesBottom[] = {
		//Bottom
		10, 14, 5,
		5, 4, 10,
};

unsigned short Renderer::IndicesTop[] = {
		//Top
		3, 2, 11,
		11, 15, 3
};