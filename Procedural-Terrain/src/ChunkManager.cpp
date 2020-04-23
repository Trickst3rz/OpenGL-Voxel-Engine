#include "ChunkManager.h"
#include "Camera.h"
#include "Timer.h"

ChunkManager::ChunkManager() : m_AmountOfChunks(33), xPosInChunk(0), zPosInChunk(0)
{
	centreOfChunks = (m_AmountOfChunks - 1) / 2;
	cachedX = -centreOfChunks;
	cachedZ = -centreOfChunks;
	currentCameraPosition.x = Camera::GetCameraPosition().x;
	currentCameraPosition.z = Camera::GetCameraPosition().z;

}

ChunkManager::~ChunkManager()
{
	m_ChunkList.clear();
	BatchVertexArray.clear();
}

void ChunkManager::UpdateAsync()
{
	
}

void ChunkManager::UpdateLoadList()
{
	int xDir = (Camera::GetCameraPosition().x) - (currentCameraPosition.x);
	int zDir = (Camera::GetCameraPosition().z) - (currentCameraPosition.z);

	if (xDir > SizeOfChunk)
	{
		xPosInChunk++;
		int offsetX = (centreOfChunks + xPosInChunk) * SizeOfChunk;
		for (int z = -centreOfChunks + zPosInChunk; z <= centreOfChunks + zPosInChunk; z++)
		{//change chunk coordinates to be correct offset
			auto start = std::chrono::high_resolution_clock::now();
			m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)] = std::make_shared<Chunk>();
			m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->SetupLandscape(centreOfChunks + xPosInChunk, z);
			m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->CreateMesh();
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = end - start;
			std::cout << duration.count() << std::endl;

			VertexBuffer BatchVertexBuffer(m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->GetVertex(), m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->GetElementCount() * sizeof * m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->GetVertex());
			VertexBufferLayout BatchLayout;
			BatchLayout.Push<GLbyte>(3);
			BatchLayout.Push<GLbyte>(3);
			//BatchLayout.Push<GLbyte>(1);
			BatchVertexArray[glm::ivec3(offsetX, 0, z * SizeOfChunk)] = std::make_shared<VertexArray>();
			BatchVertexArray[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->Bind();
			BatchVertexArray[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->AddBuffer(BatchVertexBuffer, BatchLayout);
			BatchVertexArray[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->Unbind();
		}
		currentCameraPosition.x += SizeOfChunk;
	}

	if (xDir < 0)
	{
		xPosInChunk--;
		int offsetX = (-centreOfChunks + xPosInChunk) * SizeOfChunk;
		for (int z = -centreOfChunks + zPosInChunk; z <= centreOfChunks + zPosInChunk; z++)
		{
			m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)] = std::make_shared<Chunk>();
			m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->SetupLandscape(-centreOfChunks + xPosInChunk, z);
			m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->CreateMesh();
			VertexBuffer BatchVertexBuffer(m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->GetVertex(), m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->GetElementCount() * sizeof * m_ChunkList[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->GetVertex());
			VertexBufferLayout BatchLayout;
			BatchLayout.Push<GLbyte>(3);
			BatchLayout.Push<GLbyte>(3);
			//BatchLayout.Push<GLbyte>(1);
			BatchVertexArray[glm::ivec3(offsetX, 0, z * SizeOfChunk)] = std::make_shared<VertexArray>();
			BatchVertexArray[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->Bind();
			BatchVertexArray[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->AddBuffer(BatchVertexBuffer, BatchLayout);
			BatchVertexArray[glm::ivec3(offsetX, 0, z * SizeOfChunk)]->Unbind();
		}
		currentCameraPosition.x -= SizeOfChunk;
	}

	if (zDir > SizeOfChunk)
	{
		zPosInChunk++;
		int offsetZ = (centreOfChunks + zPosInChunk) * SizeOfChunk;
		for (int x = -centreOfChunks + xPosInChunk; x <= centreOfChunks + xPosInChunk; x++)
		{//change chunk coordinates to be correct offset
			m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)] = std::make_shared<Chunk>();
			m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->SetupLandscape(x, centreOfChunks + zPosInChunk);
			m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->CreateMesh();
			VertexBuffer BatchVertexBuffer(m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->GetVertex(), m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->GetElementCount() * sizeof * m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->GetVertex());
			VertexBufferLayout BatchLayout;
			BatchLayout.Push<GLbyte>(3);
			BatchLayout.Push<GLbyte>(3);
			//BatchLayout.Push<GLbyte>(1);
			BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, offsetZ)] = std::make_shared<VertexArray>();
			BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->Bind();
			BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->AddBuffer(BatchVertexBuffer, BatchLayout);
			BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->Unbind();
		}
		currentCameraPosition.z += SizeOfChunk;
	}

	if (zDir < 0)
	{
		zPosInChunk--;
		int offsetZ = (-centreOfChunks + zPosInChunk) * SizeOfChunk;
		for (int x = -centreOfChunks + xPosInChunk; x <= centreOfChunks + xPosInChunk; x++)
		{
			m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)] = std::make_shared<Chunk>();
			m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->SetupLandscape(x, -centreOfChunks + zPosInChunk);
			m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->CreateMesh();
			VertexBuffer BatchVertexBuffer(m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->GetVertex(), m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->GetElementCount() * sizeof * m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->GetVertex());
			VertexBufferLayout BatchLayout;
			BatchLayout.Push<GLbyte>(3);
			BatchLayout.Push<GLbyte>(3);
			//BatchLayout.Push<GLbyte>(1);
			BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, offsetZ)] = std::make_shared<VertexArray>();
			BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->Bind();
			BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->AddBuffer(BatchVertexBuffer, BatchLayout);
			BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, offsetZ)]->Unbind();
		}
		currentCameraPosition.z -= SizeOfChunk;
	}
}

void ChunkManager::UpdateUnloadList()
{//Check if the current position is still inside the same chunk as the new position
	//Make sure that the next chunk is plus the size of the chunk e.g. 32
	int xDir = Camera::GetCameraPosition().x - currentCameraPosition.x;
	int zDir = Camera::GetCameraPosition().z - currentCameraPosition.z;
	
	if (xDir > SizeOfChunk)
	{
		for (int z = -centreOfChunks + zPosInChunk; z <= centreOfChunks + zPosInChunk; z++)
		{
			m_ChunkList.erase(glm::ivec3((-centreOfChunks + xPosInChunk) * SizeOfChunk, 0, z * SizeOfChunk));
			BatchVertexArray.erase(glm::ivec3(-centreOfChunks * SizeOfChunk, 0, z * SizeOfChunk));
		}
	}

	if (xDir < 0)
	{
		for (int z = -centreOfChunks + zPosInChunk; z <= centreOfChunks + zPosInChunk; z++)
		{
			m_ChunkList.erase(glm::ivec3((centreOfChunks + xPosInChunk) * SizeOfChunk, 0, z * SizeOfChunk));
			BatchVertexArray.erase(glm::ivec3((centreOfChunks + xPosInChunk) * SizeOfChunk, 0, z * SizeOfChunk));
		}
	}

	if (zDir > SizeOfChunk)
	{
		for (int x = -centreOfChunks + xPosInChunk; x <= centreOfChunks + xPosInChunk; x++)
		{
			m_ChunkList.erase(glm::ivec3(x * SizeOfChunk, 0, (-centreOfChunks + zPosInChunk) * SizeOfChunk));
			BatchVertexArray.erase(glm::ivec3(x * SizeOfChunk, 0, (-centreOfChunks + zPosInChunk) * SizeOfChunk));
		}
	}

	if (zDir < 0)
	{
		for (int x = -centreOfChunks + xPosInChunk; x <= centreOfChunks +xPosInChunk; x++)
		{
			m_ChunkList.erase(glm::ivec3(x * SizeOfChunk, 0, (centreOfChunks + zPosInChunk) * SizeOfChunk));
			BatchVertexArray.erase(glm::ivec3(x * SizeOfChunk, 0, (centreOfChunks + zPosInChunk) * SizeOfChunk));
		}
	}
}

void ChunkManager::UpdateRebuildList()
{

}

void ChunkManager::UpdateVisibilityList()
{

}

static std::mutex m_ChunksMutex;

bool ChunkManager::GenerateChunk(std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>>& chunkList, int x, int z)
{
	chunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)] = std::make_shared<Chunk>();
	std::lock_guard<std::mutex> lock(m_ChunksMutex);
	chunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->SetupLandscape(x, z);
	chunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->CreateMesh();
	return true;
}

void ChunkManager::AsyncLoadChunks()
{
	Timer timer("ChunkManager::AsyncLoadChunks");
	for (int x = -centreOfChunks; x <= centreOfChunks; x++)
	{
		for (int z = -centreOfChunks; z <= centreOfChunks; z++)
			m_Futures.push_back(std::async(std::launch::async, GenerateChunk, std::ref(m_ChunkList), x, z));
	}
}

void ChunkManager::SetupVAO(float deltaTime)
{
	if (BatchVertexArray.size() == m_AmountOfChunks * m_AmountOfChunks)
	{
		m_Futures.clear();
		return;
	}

	//Timer timer("ChunkManager::SetupVAO");
	int index = -1;

	//for (auto itr = m_Futures.begin(); itr != m_Futures.end(); itr++)
	//{
	//	if (!itr->_Is_ready())
	//		return;

	//	VertexBuffer BatchVertexBuffer(m_ChunkList[itr->_Get_value()]->GetVertex(), m_ChunkList[itr->_Get_value()]->GetElementCount() * sizeof * m_ChunkList[itr->_Get_value()]->GetVertex());
	//	VertexBufferLayout BatchLayout;
	//	BatchLayout.Push<GLbyte>(3);
	//	BatchLayout.Push<GLbyte>(3);
	//	//BatchLayout.Push<GLbyte>(1);
	//	BatchVertexArray[itr->get()] = std::make_shared<VertexArray>();
	//	BatchVertexArray[itr->get()]->Bind();
	//	BatchVertexArray[itr->get()]->AddBuffer(BatchVertexBuffer, BatchLayout);
	//	BatchVertexArray[itr->get()]->Unbind();
	//}

	for (int x = -centreOfChunks; x <= centreOfChunks; x++)
	{
		for (int z = -centreOfChunks; z <= centreOfChunks; z++)
		{
			index++;
			if (m_Futures[index].valid())
			{
				if (m_Futures[index]._Is_ready() && m_Futures[index].wait_for(std::chrono::milliseconds(25)) == std::future_status::ready)
				{
					VertexBuffer BatchVertexBuffer(m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->GetVertex(), m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->GetElementCount() * sizeof * m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->GetVertex());
					VertexBufferLayout BatchLayout;
					BatchLayout.Push<GLbyte>(3);
					BatchLayout.Push<GLbyte>(3);
					//BatchLayout.Push<GLbyte>(1);
					BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)] = std::make_shared<VertexArray>();
					BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->Bind();
					BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->AddBuffer(BatchVertexBuffer, BatchLayout);
					BatchVertexArray[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->Unbind();
				}
				else
					return;
			}
		}
	}

	/*for (auto itr = m_ChunkList.begin(); itr != m_ChunkList.end(); itr++)
	{
		index++;
		if (m_Futures[index]._Is_ready())
		{
			VertexBuffer BatchVertexBuffer(itr->second->GetVertex(), itr->second->GetElementCount() * sizeof * itr->second->GetVertex());
			VertexBufferLayout BatchLayout;
			BatchLayout.Push<GLbyte>(3);
			BatchLayout.Push<GLbyte>(3);
			BatchLayout.Push<GLbyte>(1);
			BatchVertexArray[itr->first] = std::make_shared<VertexArray>();
			BatchVertexArray[itr->first]->Bind();
			BatchVertexArray[itr->first]->AddBuffer(BatchVertexBuffer, BatchLayout);
			BatchVertexArray[itr->first]->Unbind();
			m_FuturesIndex++;
		}
		else
			return;
	}*/
}

void ChunkManager::Render(Shader& shader)
{
	for (auto itr = BatchVertexArray.begin(); itr != BatchVertexArray.end(); itr++)
	{
		shader.Bind();
		shader.SetUniform3f("u_offset", itr->first.x, 0, itr->first.z);
		m_ChunkList[itr->first]->Render(itr->second, shader);
	}
}

void ChunkManager::Update(Shader& shader)
{
	//Use Camera::GetPosition() in visablityList
	//UpdateAsync();

	//UpdateRebuildList();

	//UpdateUnloadList();

	//UpdateLoadList();

	//UpdateVisibilityList();

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

	m_AmountOfChunks = numOfChunks + 1;

	//SetupVAO();
}
