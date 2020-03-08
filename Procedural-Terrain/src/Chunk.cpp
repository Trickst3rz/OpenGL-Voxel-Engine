#include "Chunk.h"
#include "Renderer.h"

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
{//Delete the blocks
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


void Chunk::CreateMesh(Mesh* meshes[]) //Might not be const?
{
	//Render the chunk
	//Check if you can see triangle if not don't render it
	//If neighboring side is true meaning there is a cube there then don't render that side
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				if (m_Blocks[x][y][z].isActive() == false)
 					continue;

				bool lXNegative = true;
				if (x > 0)
					lXNegative = m_Blocks[x - 1][y][z].isActive();

				bool lXPositive = true;
				if (x < ChunkSize - 1)
					lXPositive = m_Blocks[x + 1][y][z].isActive();
				bool lYNegative = true;
				if (y > 0)
					lYNegative = m_Blocks[x][y - 1][z].isActive();
				bool lYPositive = true;
				if (y < ChunkSize - 1)
					lYPositive = m_Blocks[x][y + 1][z].isActive();

				bool lZNegative = true;
				if (z > 0)
					lZNegative = m_Blocks[x][y][z - 1].isActive();

				bool lZPositive = true;
				if (z < ChunkSize - 1)
					lZPositive = m_Blocks[x][y][z + 1].isActive();
				
				if (lXPositive == false || lYPositive == false || lZPositive == false || lXNegative == false || lYNegative == false || lZNegative == false)
				{
					lZPositive = lXPositive;
				}
				//CreateVoxel();
			}
		}
	}
				//Render Call and pass in NegativeX PositiveX NegativeY PositiveY NegativeZ PositiveZ
}

void Chunk::CreateVoxel(VertexBuffer& vb, IndexBuffer& ib)
{
	
}