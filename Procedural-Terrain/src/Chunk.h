#pragma once

#include "TypeDefines.h"
#include "Block.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

#include <vector>

class Chunk
{
public: 

	Chunk();

	~Chunk();

	void Render();

	void Update(float deltaTime);

	void CreateMesh();

	void SetupSphere();

	void SetupLandscape();

	void SetupAll();

	static const int ChunkSize = 32;

	unsigned int GetVBO() { return vbo; }

	int GetElementCount() { return elements; }

	Byte3* GetVertex() { return vertex; }
private:
	Block*** m_Blocks;
	//Maybe in future change from array to vector to remove and add blocks, MAYBE CHANGE m_indices to 3d array? look at using blocks etc
	//Add in placing at certain location for block later on if I do add and remove blocks
	//VertexBuffer testVBO();
	unsigned int vbo;
	int elements = 0;
	Byte3* vertex;
};

