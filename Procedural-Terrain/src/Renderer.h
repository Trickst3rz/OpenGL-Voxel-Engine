#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	static void Clear();
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const int& InstanceCount);

	static short GetDrawCalls() { return m_DrawCalls; }
	static void SetDrawCalls(int reset) { m_DrawCalls = reset; }

private:
	static short m_DrawCalls;
};

