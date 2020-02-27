#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

static class Renderer
{
public:
	static void Clear();
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};

