#pragma once

#include "TypeDefines.h"
#include "Block.h"
#include "VertexArray.h"
#include "Shader.h"

#include <vector>

struct Vertex
{
	Byte3 pos;
	Byte3 normal;
	uByte3 colour;
};

class Chunk
{
public: 

	Chunk();

	~Chunk();

	void Render(const std::shared_ptr<VertexArray> va, const Shader& shader);

	void CreateMesh();

	void SetupSphere();

	void SetupLandscape(double x, double z);

	void SetupAll();

	uByte3 GetColourType(int typeEnum);

	static const int ChunkSize = 32;

	unsigned int GetVBO() { return vbo; }

 	int GetElementCount() { return elements; }

	Vertex* GetVertex() { return vertex; }

	bool isActive() { return m_ChunkActive; }

	void SetActive(bool SetChunk) { m_ChunkActive = SetChunk; }

	Block* GetBlock(int v1, int v2, int v3) { return &m_Blocks[v1][v2][v3]; }

private:
	Block*** m_Blocks;
	unsigned int vbo = 0;
	int elements = 0;
	bool m_ChunkActive;
	Vertex* vertex;
};

