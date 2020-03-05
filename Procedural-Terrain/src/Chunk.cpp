#include "Chunk.h"

Chunk::Chunk()
{
	//Creating the chunk with many blocks
	m_Blocks = new Block**[ChunkSize];
	for (int x = 0; x < ChunkSize; x++)
	{
		m_Blocks[x] = new Block*[ChunkSize];
		for (int y = 0; y < ChunkSize; y++)
		{
			m_Blocks[x][y] = new Block[ChunkSize];
		}
	}
}

Chunk::~Chunk()
{
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			delete[] m_Blocks[x][y];
		}
		delete[] m_Blocks[x];
	}
	delete[] m_Blocks;
}

void Chunk::Update(float deltaTime)
{
	//Update removing and adding chunks use a displayList or VBO?
}

void Chunk::Render()
{
	//Render the chunk
}
