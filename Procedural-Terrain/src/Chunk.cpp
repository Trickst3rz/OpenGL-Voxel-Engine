#include "Chunk.h"
#include "noise/noise.h"
#include "noise/noiseutils.h"

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
	
	module::Perlin PerlinModule;
	double value = PerlinModule.GetValue(200.9, 100.4, 50.4);
	std::cout << value << std::endl;
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(PerlinModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(256, 256);
	heightMapBuilder.SetBounds(4.0, 8.0, 0.0, 4.0);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	//Change this depending on height to set different colour of voxels
	renderer.ClearGradient();
	renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); //Deep water
	renderer.AddGradientPoint(-0.2500, utils::Color(0, 0, 255, 255)); //Shallow water
	renderer.AddGradientPoint(0.0000, utils::Color(0, 128, 255, 255)); //Shore
	renderer.AddGradientPoint(0.0625, utils::Color(240, 240, 64, 255)); //Sand
	renderer.AddGradientPoint(0.1250, utils::Color(32, 160, 0, 255)); //Grass
	renderer.AddGradientPoint(0.3750, utils::Color(224, 224, 0, 255)); //Dirt
	renderer.AddGradientPoint(0.7500, utils::Color(128, 128, 128, 255)); //Rock
	renderer.AddGradientPoint(1.0000, utils::Color(255, 255, 255, 255)); //Snow
	renderer.EnableLight();
	renderer.SetLightContrast(3.0); //Triple contrast
	renderer.SetLightBrightness(2.0); // Double brightness
	renderer.SetLightAzimuth(180.0); //Direction of light
	renderer.SetLightElev(35.0);
	renderer.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("testHeightMap2.bmp");
	writer.WriteDestFile();

	double scale = ChunkSize;
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int z = 0; z < ChunkSize; z++)
		{	//Figure out how to use utils or noise to generate procedural generation
			double height = noise::ValueCoherentNoise3D(x * 0.1, 0, z * 0.1, 1);
			height = height * 4 + 4;
			double testHeight = heightMap.GetValue(x, z) * (double)(ChunkSize - 1);
			for (int y = 0; y < height; y++)
			{
				m_Blocks[x][y][z].SetActive(true);
			}
		}
	}
}

