#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();

	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void AddInstanceBuffer(const VertexBuffer& instanceVB, const VertexBufferLayout& layout, int index);

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;
};