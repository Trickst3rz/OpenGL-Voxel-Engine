#pragma once
#include <vector>

class IndexBuffer
{
public:

	IndexBuffer(const unsigned short* data, unsigned short count);
	~IndexBuffer();

	void Modify(std::vector<unsigned short>* data, unsigned short count);

	void Bind() const;
	void Unbind() const;

	inline unsigned short GetCount() const { return m_Count; }

private:
	unsigned int m_RendererID;
	unsigned short m_Count;
};
