#include "ChunkManager.h"
#include "Camera.h"

ChunkManager::ChunkManager() : m_AmountOfChunks(6)
{
	m_chunks = new Chunk*[m_AmountOfChunks];

	for (int x = 0; x < m_AmountOfChunks; x++)
		m_chunks[x] = new Chunk[m_AmountOfChunks];
}

ChunkManager::~ChunkManager()
{
	for (int x = 0; x < m_AmountOfChunks; x++)
		delete m_chunks[x];
	delete[] m_chunks;
}

void ChunkManager::UpdateAsync()
{

}

void ChunkManager::UpdateLoadList()
{

}

void ChunkManager::UpdateUnloadList()
{

}

void ChunkManager::UpdateRebuildList()
{

}

void ChunkManager::UpdateVisibilityList()
{

}

void ChunkManager::SetupVAO()
{
	for (int x = 0; x < m_AmountOfChunks; x++)
	{
		for (int z = 0; z < m_AmountOfChunks; z++)
		{
			m_chunks[x][z].SetupLandscape((double)x, (double)z);
			m_chunks[x][z].CreateMesh();
			VertexBuffer BatchVertexBuffer(m_chunks[x][z].GetVertex(), m_chunks[x][z].GetElementCount() * sizeof * m_chunks[x][z].GetVertex());
			VertexBufferLayout BatchLayout;
			BatchLayout.Push<GLbyte>(3);
			BatchLayout.Push<GLbyte>(3);
			//BatchLayout.Push<GLbyte>(1);
			BatchVertexArray[z + x * m_AmountOfChunks].Bind();
			BatchVertexArray[z + x * m_AmountOfChunks].AddBuffer(BatchVertexBuffer, BatchLayout);
			BatchVertexArray[z + x * m_AmountOfChunks].Unbind();
		}
	}
}

void ChunkManager::Render(Shader& shader)
{
	for (int x = 0; x < m_AmountOfChunks; x++)
	{
		for (int z = 0; z < m_AmountOfChunks; z++)
		{
			shader.Bind();
			shader.SetUniform3f("u_offset", x * m_chunks[x][z].ChunkSize, 0, -z * m_chunks[x][z].ChunkSize);
			m_chunks[x][z].Render(BatchVertexArray[z + x * m_AmountOfChunks], shader);
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
	int size = numOfChunks + numOfChunks + 1;
	m_AmountOfChunks = size * size;
}
