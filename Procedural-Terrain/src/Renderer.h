#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Chunk.h"

class Renderer
{
public:
	static void Clear();

	static void DrawInstanced(const VertexArray& va, const Shader& shader, const unsigned int& IndicesCount, const int& InstanceCount);
	static void Draw(const VertexArray& va, const Shader& shader, const int& count);
	static void DrawLineLoop(const VertexArray& va, const Shader& shader, const int& count);

	static short GetDrawCalls() { return m_DrawCalls; }
	static void SetDrawCalls(int reset) { m_DrawCalls = reset; }

	//Indice Faces
	//Maybe in future change from array to vector to remove and add blocks
	static unsigned short IndicesFront[];
	static unsigned short IndicesBack[];
	static unsigned short IndicesRight[];
	static unsigned short IndicesLeft[];
	static unsigned short IndicesBottom[];
	static unsigned short IndicesTop[];

private:
	static short m_DrawCalls;
};

