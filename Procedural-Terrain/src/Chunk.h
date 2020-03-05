#pragma once

#include "GLErrorManager.h"
#include "Block.h"

class Chunk
{
public: 

	Chunk();

	~Chunk();

	void Update(float deltaTime);

	void Render();

	static const int ChunkSize = 16;

private:
	Block*** m_Blocks;
};

