#include "Renderer.h"

void Renderer::Clear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const int& InstanceCount)
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr, InstanceCount));
	//GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));
	m_DrawCalls++;
}

short Renderer::m_DrawCalls = 0;
