#pragma once

#include "GLErrorManager.h"
#include "Block.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <vector>

class Chunk
{
public: 

	Chunk();

	~Chunk();

	void Update(float deltaTime);

	void CreateMesh(VertexBuffer& vb, IndexBuffer& ib);

	void CreateVoxel(VertexBuffer& vb, IndexBuffer& ib);

	static const int ChunkSize = 16;

private:
	Block*** m_Blocks;
	//Maybe in future change from array to vector to remove and add blocks, MAYBE CHANGE m_indices to 3d array? look at using blocks etc
	//Add in placing at certain location for block later on if I do add and remove blocks
	std::vector<unsigned short> m_indices;
};

