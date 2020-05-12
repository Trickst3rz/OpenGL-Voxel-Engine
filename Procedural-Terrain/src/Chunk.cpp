#include "Chunk.h"
#include "noise/noise.h"
#include "noise/noiseutils.h"
#include "Renderer.h"
#include "Timer.h"

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

void Chunk::Render(const std::shared_ptr<VertexArray> va, const Shader& shader)
{
	if (!elements)
		return;

	Renderer::Draw(*va, shader, GetElementCount());
}

void Chunk::CreateMesh()
{
	//Check if you can see triangle if not don't render it
	//If neighboring side is true meaning there is a cube there then don't render that side

	int i = 0;

	vertex = new Vertex[ChunkSize * ChunkSize * ChunkSize * 6];

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
				
				if (!lXNegative && m_Blocks[x][y][z].GetFace(LEFT) == LEFT || !Global::GetInstance().OcclusionCulling) //Left Face, check if to not render this face(e.g. occluded by another chunk
				{	//pos										//normal							//Colour
					vertex[i].pos = Byte3(x, y, z),				vertex[i].normal = Byte3(-1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y, z + 1),			vertex[i].normal = Byte3(-1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y + 1, z),			vertex[i].normal = Byte3(-1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y + 1, z),			vertex[i].normal = Byte3(-1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y, z + 1),			vertex[i].normal = Byte3(-1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y + 1, z + 1),		vertex[i].normal = Byte3(-1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType());
				}
				if (!lXPositive && m_Blocks[x][y][z].GetFace(RIGHT) == RIGHT || !Global::GetInstance().OcclusionCulling) //Right Face, check if to not render this face(e.g. occluded by another chunk
				{	//pos										//normal							//Colour
					vertex[i].pos = Byte3(x + 1, y, z),			vertex[i].normal = Byte3(1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y + 1, z),		vertex[i].normal = Byte3(1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y, z + 1),		vertex[i].normal = Byte3(1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y + 1, z),		vertex[i].normal = Byte3(1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y + 1, z + 1),	vertex[i].normal = Byte3(1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y, z + 1),		vertex[i].normal = Byte3(1, 0, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType());
				}												
				if (!lYNegative && m_Blocks[x][y][z].GetFace(BOTTOM) == BOTTOM || !Global::GetInstance().OcclusionCulling) //Bottom Face, check if to not render this face(e.g. occluded by another chunk
				{	//pos										//normal							//Colour
					vertex[i].pos = Byte3(x, y, z),				vertex[i].normal = Byte3(0, -1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y, z),			vertex[i].normal = Byte3(0, -1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y, z + 1),		vertex[i].normal = Byte3(0, -1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y, z + 1),		vertex[i].normal = Byte3(0, -1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y, z + 1),			vertex[i].normal = Byte3(0, -1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y, z),				vertex[i].normal = Byte3(0, -1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType());
				}												
				if (!lYPositive && m_Blocks[x][y][z].GetFace(TOP) == TOP || !Global::GetInstance().OcclusionCulling) //Top Face, check if to not render this face(e.g. occluded by another chunk
				{	//pos										//normal							//Colour
					vertex[i].pos = Byte3(x + 1, y + 1, z),		vertex[i].normal = Byte3(0, 1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y + 1, z),			vertex[i].normal = Byte3(0, 1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y + 1, z + 1),		vertex[i].normal = Byte3(0, 1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y + 1, z + 1),		vertex[i].normal = Byte3(0, 1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y + 1, z + 1),	vertex[i].normal = Byte3(0, 1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y + 1, z),		vertex[i].normal = Byte3(0, 1, 0),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType());
				}												
				if (!lZNegative && m_Blocks[x][y][z].GetFace(FRONT) == FRONT || !Global::GetInstance().OcclusionCulling) //Front Face, check if to not render this face(e.g. occluded by another chunk
				{	//pos										//normal						//Colour
					vertex[i].pos = Byte3(x, y, z),				vertex[i].normal = Byte3(0, 0, -1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y + 1, z),			vertex[i].normal = Byte3(0, 0, -1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y, z),			vertex[i].normal = Byte3(0, 0, -1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y + 1, z),			vertex[i].normal = Byte3(0, 0, -1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y + 1, z),		vertex[i].normal = Byte3(0, 0, -1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y, z),			vertex[i].normal = Byte3(0, 0, -1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType());
				}												
				if (!lZPositive && m_Blocks[x][y][z].GetFace(BACK) == BACK || !Global::GetInstance().OcclusionCulling) //Back Face, check if to not render this face(e.g. occluded by another chunk
				{	//pos										//normal							//Colour
					vertex[i].pos = Byte3(x + 1, y, z + 1),		vertex[i].normal = Byte3(0, 0, 1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y + 1, z + 1),	vertex[i].normal = Byte3(0, 0, 1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y, z + 1),			vertex[i].normal = Byte3(0, 0, 1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x + 1, y + 1, z + 1),	vertex[i].normal = Byte3(0, 0, 1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y + 1, z + 1),		vertex[i].normal = Byte3(0, 0, 1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType()),
					vertex[i].pos = Byte3(x, y, z + 1),			vertex[i].normal = Byte3(0, 0, 1),	vertex[i++].colour = GetColourType(m_Blocks[x][y][z].GetType());
				}												
			}
		}
	}
	elements = i;
}

//Creates a sphere
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

//Creates a cube
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

uByte3 Chunk::GetColourType(int typeEnum)
{
	switch (typeEnum)
	{
	case BLOCK_DEEP_WATER:
		return uByte3(0, 0, 128);
	case BLOCK_SHALLOW_WATER:
		return uByte3(0, 0, 255);
	case BLOCK_SHORE:
		return uByte3(0, 128, 255);
	case BLOCK_SAND:
		return uByte3(240, 240, 64);
	case BLOCK_GRASS:
		return uByte3(32, 160, 0);
	case BLOCK_DIRT:
		return uByte3(92, 58, 0);
	case BLOCK_ROCK:
		return uByte3(128, 128, 128);
	case BLOCK_SNOW:
		return uByte3(255, 255, 255);
	}
	std::cout << "Error invalid colour type" << std::endl;
	return uByte3(0, 0, 0);
}

void Chunk::SetupLandscape(double x, double z)
{	//Setup the landscape with Perlin noise
	//Create a Perlin module, set the seed, frequency and Octaves
	module::Perlin PerlinModule;
	PerlinModule.SetSeed(Global::GetInstance().Seed);
	PerlinModule.SetFrequency(Global::GetInstance().Frequency);
	PerlinModule.SetOctaveCount(Global::GetInstance().OctaveCount);

	std::cout << sizeof(m_Blocks[0][0][0]) << std::endl;

	//Create a 2D heightmap
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;

	//Set a heightmap with Perlin noise and set the size based on the chunk size
	heightMapBuilder.SetSourceModule(PerlinModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(ChunkSize, ChunkSize);

	//Create the x,z coordinates 
	double upperX = x + 1.0;
	double upperZ = z + 1.0;
	double lowerX = x;
	double lowerZ = z;

	//Set the coordinates to the heightmap so the whole map is used for this chunk
	heightMapBuilder.SetBounds(lowerX, upperX, lowerZ, upperZ);
	heightMapBuilder.Build();
	
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int z = 0; z < ChunkSize; z++)
		{
			float HeightValue = (heightMap.GetValue(x, z) + 1.0f) * 0.5f; //Get the height map x, z coordinates then convert range from -1-1 to 0-1
			float Height = (HeightValue * (ChunkSize - 1) * 1.0f) * 1.0f;

			if (Height > ChunkSize - 1)
				Height = ChunkSize;

			if (Height < 0.0f)
				Height = 0.1f;
			
			//From the height value coordinate set blocks active based on the height of the heightmap
			for (int y = 0; y < Height; y++)
			{
				m_Blocks[x][y][z].SetActive(true);
				
				//Set the type of block based on the height
				if (y >= 6.0f && y < 10.0f)
					m_Blocks[x][y][z].SetType(BLOCK_SHALLOW_WATER);
				else if (y >= 10.0f && y < 14.0f)
					m_Blocks[x][y][z].SetType(BLOCK_SHORE);
				else if (y >= 14.0f && y < 19.0f)
					m_Blocks[x][y][z].SetType(BLOCK_SAND);
				else if (y >= 19.0f && y < 24.0f)
					m_Blocks[x][y][z].SetType(BLOCK_GRASS);
				else if (y >= 24.0f && y < 27.0f)
					m_Blocks[x][y][z].SetType(BLOCK_DIRT);
				else if (y >= 27.0f && y < 30.0f)
					m_Blocks[x][y][z].SetType(BLOCK_ROCK);
				else if (y >= 30.0f)
					m_Blocks[x][y][z].SetType(BLOCK_SNOW);
			}
		}
	}
}

