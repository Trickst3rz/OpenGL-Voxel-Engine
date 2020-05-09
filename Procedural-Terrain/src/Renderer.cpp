#include "Renderer.h"

short Renderer::m_DrawCalls = 0;

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

void Renderer::DrawLineLoop(const VertexArray& va, const Shader& shader, const int& count)
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArrays(GL_LINE_LOOP, 0, count));
	m_DrawCalls++;
}