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
	delete[] vertex;
}

void Chunk::Render()
{
	if (!elements)
		return;

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 0, 0));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, elements));
}

void Chunk::Update(float deltaTime)
{
	//Update removing and adding chunks use a displayList or VBO?
}


void Chunk::CreateMesh() //Might not be const?
{
	//Create Mesh
	//Check if you can see triangle if not don't render it
	//If neighboring side is true meaning there is a cube there then don't render that side
	int i = 0;

	vertex = new Byte3[ChunkSize * ChunkSize * ChunkSize * 6 * 6];

	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				if (m_Blocks[x][y][z].isActive() == false)
 					continue;

				bool lXNegative = false;
				if (x > 0)
					lXNegative = m_Blocks[x - 1][y][z].isActive();

				bool lXPositive = false;
				if (x < ChunkSize - 1)
					lXPositive = m_Blocks[x + 1][y][z].isActive();

				bool lYNegative = false;
				if (y > 0)
					lYNegative = m_Blocks[x][y - 1][z].isActive();

				bool lYPositive = false;
				if (y < ChunkSize - 1)
					lYPositive = m_Blocks[x][y + 1][z].isActive();

				bool lZNegative = false;
				if (z > 0)
					lZNegative = m_Blocks[x][y][z - 1].isActive();

				bool lZPositive = false;
				if(z < ChunkSize - 1)
					lZPositive = m_Blocks[x][y][z + 1].isActive();
				
				if (!lXNegative) //Left Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x, y, z),				vertex[i++] = Byte3(-1, 0, 0),
					vertex[i++] = Byte3(x, y, z + 1),			vertex[i++] = Byte3(-1, 0, 0),
					vertex[i++] = Byte3(x, y + 1, z),			vertex[i++] = Byte3(-1, 0, 0),
					vertex[i++] = Byte3(x, y + 1, z),			vertex[i++] = Byte3(-1, 0, 0),
					vertex[i++] = Byte3(x, y, z + 1),			vertex[i++] = Byte3(-1, 0, 0),
					vertex[i++] = Byte3(x, y + 1, z + 1),		vertex[i++] = Byte3(-1, 0, 0);
				}
				if (!lXPositive) //Right Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x + 1, y, z),			vertex[i++] = Byte3(1, 0, 0),
					vertex[i++] = Byte3(x + 1, y + 1, z),		vertex[i++] = Byte3(1, 0, 0),
					vertex[i++] = Byte3(x + 1, y, z + 1),		vertex[i++] = Byte3(1, 0, 0),
					vertex[i++] = Byte3(x + 1, y + 1, z),		vertex[i++] = Byte3(1, 0, 0),
					vertex[i++] = Byte3(x + 1, y + 1, z + 1),	vertex[i++] = Byte3(1, 0, 0),
					vertex[i++] = Byte3(x + 1, y, z + 1),		vertex[i++] = Byte3(1, 0, 0);
				}												
				if (!lYNegative) //Bottom Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x, y, z),				vertex[i++] = Byte3(0, -1, 0),
					vertex[i++] = Byte3(x + 1, y, z),			vertex[i++] = Byte3(0, -1, 0),
					vertex[i++] = Byte3(x + 1, y, z + 1),		vertex[i++] = Byte3(0, -1, 0),
					vertex[i++] = Byte3(x + 1, y, z + 1),		vertex[i++] = Byte3(0, -1, 0),
					vertex[i++] = Byte3(x, y, z + 1),			vertex[i++] = Byte3(0, -1, 0),
					vertex[i++] = Byte3(x, y, z),				vertex[i++] = Byte3(0, -1, 0);
				}												
				if (!lYPositive) //Top Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x + 1, y + 1, z),		vertex[i++] = Byte3(0, 1, 0),
					vertex[i++] = Byte3(x, y + 1, z),			vertex[i++] = Byte3(0, 1, 0),
					vertex[i++] = Byte3(x, y + 1, z + 1),		vertex[i++] = Byte3(0, 1, 0),
					vertex[i++] = Byte3(x, y + 1, z + 1),		vertex[i++] = Byte3(0, 1, 0),
					vertex[i++] = Byte3(x + 1, y + 1, z + 1),	vertex[i++] = Byte3(0, 1, 0),
					vertex[i++] = Byte3(x + 1, y + 1, z),		vertex[i++] = Byte3(0, 1, 0);
				}												
				if (!lZNegative) //Front Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x, y, z),				vertex[i++] = Byte3(0, 0, -1),
					vertex[i++] = Byte3(x, y + 1, z),			vertex[i++] = Byte3(0, 0, -1),
					vertex[i++] = Byte3(x + 1, y, z),			vertex[i++] = Byte3(0, 0, -1),
					vertex[i++] = Byte3(x, y + 1, z),			vertex[i++] = Byte3(0, 0, -1),
					vertex[i++] = Byte3(x + 1, y + 1, z),		vertex[i++] = Byte3(0, 0, -1),
					vertex[i++] = Byte3(x + 1, y, z),			vertex[i++] = Byte3(0, 0, -1);
				}												
				if (!lZPositive) //Back Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x + 1, y, z + 1),		vertex[i++] = Byte3(0, 0, 1),
					vertex[i++] = Byte3(x + 1, y + 1, z + 1),	vertex[i++] = Byte3(0, 0, 1),
					vertex[i++] = Byte3(x, y, z + 1),			vertex[i++] = Byte3(0, 0, 1),
					vertex[i++] = Byte3(x + 1, y + 1, z + 1),	vertex[i++] = Byte3(0, 0, 1),
					vertex[i++] = Byte3(x, y + 1, z + 1),		vertex[i++] = Byte3(0, 0, 1),
					vertex[i++] = Byte3(x, y, z + 1),			vertex[i++] = Byte3(0, 0, 1);
				}												
			}
		}
	}
	elements = i;
}

void Chunk::SetupSphere()
{
	for (int z = 0; z < ChunkSize; z++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int x = 0; x < ChunkSize; x++)
			{
				if (sqrt((float)(x - ChunkSize / 2) * (x - ChunkSize / 2) + (y - ChunkSize / 2) * (y - ChunkSize / 2) + (z - ChunkSize / 2) * (z - ChunkSize / 2) <= ChunkSize/2))
				{
					m_Blocks[x][y][z].SetActive(true);
				}
			}
		}
	}
}

void Chunk::SetupAll()
{
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				m_Blocks[x][y][z].SetActive(true);
			}
		}
	}
}

void Chunk::SetupLandscape()
{

}

