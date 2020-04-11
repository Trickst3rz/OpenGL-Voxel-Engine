#include "ChunkManager.h"

ChunkManager::ChunkManager()
{

	m_chunks = new Chunk*[AmountOfChunks];

	for (int x = 0; x < AmountOfChunks; x++)
		m_chunks[x] = new Chunk[AmountOfChunks];
}

ChunkManager::~ChunkManager()
{
	for (int x = 0; x < AmountOfChunks; x++)
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
	for (int x = 0; x < AmountOfChunks; x++)
	{
		for (int z = 0; z < AmountOfChunks; z++)
		{
			m_chunks[x][z].SetupLandscape((double)x, (double)z);
			m_chunks[x][z].CreateMesh();
			VertexBuffer BatchVertexBuffer(m_chunks[x][z].GetVertex(), m_chunks[x][z].GetElementCount() * sizeof * m_chunks[x][z].GetVertex());
			VertexBufferLayout BatchLayout;
			BatchLayout.Push<GLbyte>(3);
			BatchLayout.Push<GLbyte>(3);
			//BatchLayout.Push<GLbyte>(1);
			BatchVertexArray[z + x * AmountOfChunks].Bind();
			BatchVertexArray[z + x * AmountOfChunks].AddBuffer(BatchVertexBuffer, BatchLayout);
			BatchVertexArray[z + x * AmountOfChunks].Unbind();
		}
	}
}

void ChunkManager::Render(Shader& shader)
{
	for (int x = 0; x < AmountOfChunks; x++)
	{
		for (int z = 0; z < AmountOfChunks; z++)
		{
			shader.Bind();
			shader.SetUniform3f("u_offset", x * m_chunks[x][z].ChunkSize, 0, z * m_chunks[x][z].ChunkSize);
			m_chunks[x][z].Render(BatchVertexArray[z + x * AmountOfChunks], shader);
		}
	}
}

void ChunkManager::Update()
{

}
