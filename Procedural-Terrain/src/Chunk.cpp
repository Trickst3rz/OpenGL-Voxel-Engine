#include "Chunk.h"
#include "noise/noise.h"
#include "noise/noiseutils.h"
#include "Renderer.h"
#include "Timer.h";

Chunk::Chunk() : m_ChunkActive(false)
{
	//Creating the chunk with many blocks
	m_Blocks = new Block** [ChunkSize];
	for (int x = 0; x < ChunkSize; x++)
	{
		m_Blocks[x] = new Block * [ChunkSize];
		for (int y = 0; y < ChunkSize; y++)
		{
			m_Blocks[x][y] = new Block[ChunkSize];
		}
	}
}

Chunk::~Chunk()
{//Delete the blocks, change this to a smart pointer in the future since looping through takes awhile to close program
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

void Chunk::Render(const std::shared_ptr<VertexArray> va, const Shader& shader)
{
	if (!elements)
		return;

	Renderer::Draw(*va, shader, GetElementCount());
}

void Chunk::Update(float deltaTime)
{
	//Update removing and adding chunks use a displayList or VBO?
}


void Chunk::CreateMesh() //Might not be const? //MAKE THE m_blocks a 1D array instead of 3D array as it is alot faster 
{
	//Create Mesh
	//Check if you can see triangle if not don't render it
	//If neighboring side is true meaning there is a cube there then don't render that side
	int i = 0;

	vertex = new Byte3[ChunkSize * ChunkSize * ChunkSize * 6 * 6 * 6];

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
					vertex[i++] = Byte3(x, y, z),				vertex[i++] = Byte3(-1, 0, 0), vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y, z + 1),			vertex[i++] = Byte3(-1, 0, 0), vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y + 1, z),			vertex[i++] = Byte3(-1, 0, 0), vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y + 1, z),			vertex[i++] = Byte3(-1, 0, 0), vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y, z + 1),			vertex[i++] = Byte3(-1, 0, 0), vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y + 1, z + 1),		vertex[i++] = Byte3(-1, 0, 0), vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType());
				}
				if (!lXPositive) //Right Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x + 1, y, z),			vertex[i++] = Byte3(1, 0, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y + 1, z),		vertex[i++] = Byte3(1, 0, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y, z + 1),		vertex[i++] = Byte3(1, 0, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y + 1, z),		vertex[i++] = Byte3(1, 0, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y + 1, z + 1),	vertex[i++] = Byte3(1, 0, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y, z + 1),		vertex[i++] = Byte3(1, 0, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType());
				}												
				if (!lYNegative) //Bottom Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x, y, z),				vertex[i++] = Byte3(0, -1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y, z),			vertex[i++] = Byte3(0, -1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y, z + 1),		vertex[i++] = Byte3(0, -1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y, z + 1),		vertex[i++] = Byte3(0, -1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y, z + 1),			vertex[i++] = Byte3(0, -1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y, z),				vertex[i++] = Byte3(0, -1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType());
				}												
				if (!lYPositive) //Top Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x + 1, y + 1, z),		vertex[i++] = Byte3(0, 1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y + 1, z),			vertex[i++] = Byte3(0, 1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y + 1, z + 1),		vertex[i++] = Byte3(0, 1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y + 1, z + 1),		vertex[i++] = Byte3(0, 1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y + 1, z + 1),	vertex[i++] = Byte3(0, 1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y + 1, z),		vertex[i++] = Byte3(0, 1, 0),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType());
				}												
				if (!lZNegative) //Front Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x, y, z),				vertex[i++] = Byte3(0, 0, -1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y + 1, z),			vertex[i++] = Byte3(0, 0, -1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y, z),			vertex[i++] = Byte3(0, 0, -1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y + 1, z),			vertex[i++] = Byte3(0, 0, -1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y + 1, z),		vertex[i++] = Byte3(0, 0, -1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y, z),			vertex[i++] = Byte3(0, 0, -1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType());
				}												
				if (!lZPositive) //Back Face
				{	//pos										//normal						//Add get type of brick in the future
					vertex[i++] = Byte3(x + 1, y, z + 1),		vertex[i++] = Byte3(0, 0, 1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y + 1, z + 1),	vertex[i++] = Byte3(0, 0, 1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y, z + 1),			vertex[i++] = Byte3(0, 0, 1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x + 1, y + 1, z + 1),	vertex[i++] = Byte3(0, 0, 1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y + 1, z + 1),		vertex[i++] = Byte3(0, 0, 1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i++] = Byte3(x, y, z + 1),			vertex[i++] = Byte3(0, 0, 1),	vertex[i++] = GetColourType(m_Blocks[x][y][z].GetType());
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
				m_Blocks[0][0][z].SetActive(true);
			}
		}
	}
}

Byte3 Chunk::GetColourType(int typeEnum)
{
	switch (typeEnum)
	{
	case 0:
		return Byte3(0, 0, 126);
	case 1: 
		return Byte3(0, 0, 40);
	case 2:
		return Byte3(0, 127, 46);
	case 3:
		return Byte3(127, 127, 64);
	case 4:
		return Byte3(32, 127, 0);
	case 5:
		return Byte3(127, 127, 0);
	case 6:
		return Byte3(64, 64, 64);
	case 7:
		return Byte3(127, 127, 127);
	}
}

void Chunk::SetupLandscape(double x, double z)
{
	module::Perlin PerlinModule;
	PerlinModule.SetSeed(Global::GetSeed()); //In the future change value from a hard coded value to random each time or change with gizmos e.g. seed?
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(PerlinModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(ChunkSize, ChunkSize);
	upperX = x + 1.0;
	upperZ = z + 1.0;
	lowerX = x;
	lowerZ = z;
	heightMapBuilder.SetBounds(lowerX, upperX, lowerZ, upperZ);
	heightMapBuilder.Build();
	
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int z = 0; z < ChunkSize; z++)
		{
			float HeightValue = (heightMap.GetValue(x, z) + 1.0f) * 0.5f; //Get the height map x, z coordinates then convert range from -1-1 to 0-1
			float Height = (HeightValue * (ChunkSize - 1) * 1.0f) * 1.0f;

			if (Height > 31.0f)
				Height = 31.0f;

			if (Height < 0.0f)
				Height = 0.1f;
			
			for (int y = 0; y < Height; y++)
			{
				m_Blocks[x][y][z].SetActive(true);
				
				if (HeightValue >= 0.375f && HeightValue < 0.5f)
					m_Blocks[x][y][z].SetType(BLOCK_SHALLOW_WATER);
				else if (HeightValue >= 0.5f && HeightValue < 0.532f)
					m_Blocks[x][y][z].SetType(BLOCK_SHORE);
				else if (HeightValue >= 0.532f && HeightValue < 0.563f)
					m_Blocks[x][y][z].SetType(BLOCK_SAND);
				else if (HeightValue >= 0.563f && HeightValue < 0.689f)
					m_Blocks[x][y][z].SetType(BLOCK_GRASS);
				else if (HeightValue >= 0.689f && HeightValue < 0.875f)
					m_Blocks[x][y][z].SetType(BLOCK_DIRT);
				else if (HeightValue >= 0.0875f && HeightValue < 0.95f)
					m_Blocks[x][y][z].SetType(BLOCK_ROCK);
				else if (HeightValue >= 0.95f)
					m_Blocks[x][y][z].SetType(BLOCK_SNOW);

			}
		}
	}
}

