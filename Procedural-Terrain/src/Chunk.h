#pragma once

#include "TypeDefines.h"
#include "Block.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

#include <vector>

class Chunk
{
public: 

	Chunk();

	~Chunk();

	void Render(const std::shared_ptr<VertexArray> va, const Shader& shader);

	void Update(float deltaTime);

	void CreateMesh();

	void SetupSphere();

	void SetupLandscape(double x, double z);

	void SetupAll();

	Byte3 GetColourType(int typeEnum);

	static const int ChunkSize = 32;

	unsigned int GetVBO() { return vbo; }

	int GetElementCount() { return elements; }

	Byte3* GetVertex() { return vertex; }

	bool isActive() { return m_ChunkActive; }

	void SetActive(bool SetChunk) { m_ChunkActive = SetChunk; }

	Block* GetBlock(int v1, int v2, int v3) { return &m_Blocks[v1][v2][v3]; }

private:
	Block*** m_Blocks;
	//Maybe in future change from array to vector to remove and add blocks, MAYBE CHANGE m_indices to 3d array? look at using blocks etc
	//Add in placing at certain location for block later on if I do add and remove blocks
	//VertexBuffer testVBO();
	unsigned int vbo = 0;
	int elements = 0;
	bool m_ChunkActive;
	Byte3* vertex;
	double lowerX;
	double upperX;
	double lowerZ;
	double upperZ;
};

