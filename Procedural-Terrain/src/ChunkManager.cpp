#include "ChunkManager.h"
#include "Camera.h"
#include <math.h>

ChunkManager::ChunkManager() : m_AmountOfChunks(9)
{
	centreOfChunks = (m_AmountOfChunks - 1) / 2;

	currentCameraPosition.x = Camera::GetCameraPosition().x;
	currentCameraPosition.z = Camera::GetCameraPosition().z;

	m_chunks = new Chunk*[m_AmountOfChunks];

	for (int x = 0; x < m_AmountOfChunks; x++)
		m_chunks[x] = new Chunk[m_AmountOfChunks];	

}

ChunkManager::~ChunkManager()
{
	for (int x = 0; x < m_AmountOfChunks; x++)
		delete m_chunks[x];
	delete[] m_chunks;

	m_ChunkList.clear();
}

void ChunkManager::UpdateAsync()
{
	
}

void ChunkManager::UpdateLoadList()
{
	

}

void ChunkManager::UpdateUnloadList()
{//Check if the current position is still inside the same chunk as the new position
	//Make sure that the next chunk is plus the size of the chunk e.g. 32
	if (currentCameraPosition.x != std::floor(Camera::GetCameraPosition().x) || currentCameraPosition.z != std::floor(Camera::GetCameraPosition().z))
	{

	}
}

void ChunkManager::UpdateRebuildList()
{

}

void ChunkManager::UpdateVisibilityList()
{

}

void ChunkManager::SetupVAO()
{
	for (int x = -centreOfChunks; x <= centreOfChunks; x++)
	{
		for (int z = -centreOfChunks; z <= centreOfChunks; z++)
		{
			m_ChunkList[glm::ivec3(x, 0, z)] = std::make_shared<Chunk>();
			m_ChunkList[glm::ivec3(x, 0, z)]->SetupLandscape(x, z);
			m_ChunkList[glm::ivec3(x, 0, z)]->CreateMesh();
			VertexBuffer BatchVertexBuffer(m_ChunkList[glm::ivec3(x, 0, z)]->GetVertex(), m_ChunkList[glm::ivec3(x, 0, z)]->GetElementCount() * sizeof * m_ChunkList[glm::ivec3(x, 0, z)]->GetVertex());
			/*m_chunks[x][z].SetupLandscape((double)x, (double)z);
			m_chunks[x][z].CreateMesh();
			m_chunks[x][z].SetCameraPosX(x + Camera::GetCameraPosition().x);
			m_chunks[x][z].SetCameraPosZ(-z + Camera::GetCameraPosition().z);*/
			//VertexBuffer BatchVertexBuffer(m_chunks[x][z].GetVertex(), m_chunks[x][z].GetElementCount() * sizeof * m_chunks[x][z].GetVertex());
			VertexBufferLayout BatchLayout;
			BatchLayout.Push<GLbyte>(3);
			BatchLayout.Push<GLbyte>(3);
			//BatchLayout.Push<GLbyte>(1);
			BatchVertexArray[(z + centreOfChunks) + (x + centreOfChunks) * m_AmountOfChunks].Bind();
			BatchVertexArray[(z + centreOfChunks) + (x + centreOfChunks) * m_AmountOfChunks].AddBuffer(BatchVertexBuffer, BatchLayout);
			BatchVertexArray[(z + centreOfChunks) + (x + centreOfChunks) * m_AmountOfChunks].Unbind();
		}
	}
}

void ChunkManager::Render(Shader& shader)
{
	for (int x = -centreOfChunks; x <= centreOfChunks; x++)
	{
		for (int z = -centreOfChunks; z <= centreOfChunks; z++)
		{
			shader.Bind();
			shader.SetUniform3f("u_offset", (x + currentCameraPosition.x) * m_ChunkList[glm::ivec3(x, 0, z)]->ChunkSize, 0, (z + currentCameraPosition.z) * m_ChunkList[glm::ivec3(x, 0, z)]->ChunkSize);
			m_ChunkList[glm::ivec3(x, 0, z)]->Render(BatchVertexArray[(z + centreOfChunks) + (x + centreOfChunks) * m_AmountOfChunks], shader);
		}
	}
}

void ChunkManager::Update(Shader& shader)
{
	//Use Camera::GetPosition() in visablityList
	UpdateAsync();

	UpdateLoadList();

	SetupVAO();

	UpdateRebuildList();

	UpdateUnloadList();

	UpdateVisibilityList();

	//if (cameraPosition != m_cameraPosition)
	//{
	//	//Do frustum culling to check if the chunk should be rendered
	//	Render(shader);
	//}

	Render(shader);
}

void ChunkManager::SetChunkDistance(int numOfChunks)
{//Keep amount of chunks as a const value for now
	//Maybe add a button to ImGui that you click to load the terrain again
	for (int x = 0; x < m_AmountOfChunks; x++)
		delete m_chunks[x];
	delete[] m_chunks;

	int size = numOfChunks + numOfChunks + 1;
	m_AmountOfChunks = size * size;

	m_chunks = new Chunk * [m_AmountOfChunks];

	for (int x = 0; x < m_AmountOfChunks; x++)
		m_chunks[x] = new Chunk[m_AmountOfChunks];

	SetupVAO();
}
