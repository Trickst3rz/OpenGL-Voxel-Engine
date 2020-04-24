#include "ChunkManager.h"
#include "Camera.h"
#include "Timer.h"
#include <condition_variable>

static std::mutex m_ChunksMutex;
static std::mutex m_UnloadMutex;
static std::mutex m_LoadMutex;
static std::mutex m_RubuildMutex;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_ChunkList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_LoadList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_UnloadList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_RebuildList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_VisibilityList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_SetupList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_RenderList;
static std::unordered_map<glm::ivec3, std::shared_ptr<VertexArray>> BatchVertexArray;

static int m_AmountOfChunks; //Amount of chunks in every direction just a test temp variable atm to see if it works REMOVE LATER ON MAYBE CHANGE TO STATIC?
static int centreOfChunks;
static int xPosInChunk; //Current x axis position in chunk coordinates
static int zPosInChunk; //Current z axis position in chunk coordinates

static std::vector<std::shared_ptr<std::future<void>>> m_Futures;
static std::vector<std::shared_ptr<std::future<void>>> m_UpdateFutures;

static glm::ivec3 currentCameraPosition;

ChunkManager::ChunkManager()
{
	m_AmountOfChunks = 9;
	xPosInChunk = 0;
	zPosInChunk = 0;
	centreOfChunks = (m_AmountOfChunks - 1) / 2;

	currentCameraPosition.x = Camera::GetCameraPosition().x;
	currentCameraPosition.z = Camera::GetCameraPosition().z;
}

ChunkManager::~ChunkManager()
{
	m_ChunkList.clear();
	BatchVertexArray.clear();
	m_Futures.clear();
	m_UpdateFutures.clear();
}

void ChunkManager::UpdateAllAsync()
{
	UpdateUnloadList();
	//UpdateLoadList();
	//UpdateRebuildList();
}

void ChunkManager::UpdateAsync()
{
	m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, UpdateAllAsync)));
}

void ChunkManager::LoadChunksZDir(int OffsetZ, bool isLoadChunk) //if false delete chunk if true load chunk
{
	if (isLoadChunk)
	{
		std::lock_guard<std::mutex> lock(m_LoadMutex);
		for (int x = -centreOfChunks + xPosInChunk; x <= centreOfChunks + xPosInChunk; x++)
		{
			m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)] = std::make_shared<Chunk>();
			m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)]->SetupLandscape(x, OffsetZ);
			m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)]->CreateMesh();
			m_LoadList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)] = m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)];
			m_ChunkList.erase(glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk));
		}
	}
	else
	{
		std::lock_guard<std::mutex> lock(m_UnloadMutex);
		for (int x = -centreOfChunks + xPosInChunk; x <= centreOfChunks + xPosInChunk; x++)
		{
			m_SetupList.erase(glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk));
			BatchVertexArray.erase(glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk));
		}
	}
}

void ChunkManager::LoadChunksXDir(int OffsetX, bool isLoadChunk)
{
	switch (isLoadChunk)
	{
	case true:
	{
		std::lock_guard<std::mutex> lock(m_ChunksMutex);
		for (int z = -centreOfChunks + zPosInChunk; z <= centreOfChunks + zPosInChunk; z++)
		{
			m_ChunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)] = std::make_shared<Chunk>();
			m_ChunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)]->SetupLandscape(OffsetX, z);
			m_ChunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)]->CreateMesh();
			m_LoadList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)] = m_ChunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)];
			m_ChunkList.erase(glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk));
		}
		break;
	}
	case false:
	{
		std::lock_guard<std::mutex> lock(m_UnloadMutex);
		for (int z = -centreOfChunks + zPosInChunk; z <= centreOfChunks + zPosInChunk; z++)
		{
			m_SetupList.erase(glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk));
			BatchVertexArray.erase(glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk));
		}
		break;
	}
	}
}

void ChunkManager::UpdateLoadList()
{
	int xDir = (Camera::GetCameraPosition().x) - (currentCameraPosition.x);
	int zDir = (Camera::GetCameraPosition().z) - (currentCameraPosition.z);

	bool isload = true;

	if (xDir > SizeOfChunk)
	{
		Timer timer("ChunkManager::UpdateLoadList");
		xPosInChunk++;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksXDir, centreOfChunks + xPosInChunk, isload)));
		currentCameraPosition.x += SizeOfChunk;
	}

	if (xDir < 0)
	{
		Timer timer("ChunkManager::UpdateLoadList");
		xPosInChunk--;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksXDir, -centreOfChunks + xPosInChunk, isload)));
		currentCameraPosition.x -= SizeOfChunk;
	}

	if (zDir > SizeOfChunk)
	{
		Timer timer("ChunkManager::UpdateLoadList");
		zPosInChunk++;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksZDir, centreOfChunks + zPosInChunk, isload)));
		currentCameraPosition.z += SizeOfChunk;
	}

	if (zDir < 0)
	{
		Timer timer("ChunkManager::UpdateLoadList");
		zPosInChunk--;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksZDir, -centreOfChunks + zPosInChunk, isload)));
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
		Timer timer("ChunkManager::UpdateUnloadList");
		bool isUnload = false;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksXDir, -centreOfChunks + xPosInChunk, isUnload)));
	}

	if (xDir < 0)
	{
		Timer timer("ChunkManager::UpdateUnloadList");
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksXDir, centreOfChunks + xPosInChunk, false)));
	}

	if (zDir > SizeOfChunk)
	{
		Timer timer("ChunkManager::UpdateUnloadList");
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksZDir, -centreOfChunks + zPosInChunk, false)));
	}

	if (zDir < 0)
	{
		Timer timer("ChunkManager::UpdateUnloadList");
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksZDir, centreOfChunks + zPosInChunk, false)));
	}
}

void ChunkManager::UpdateRebuildList()
{

}

void ChunkManager::UpdateVisibilityList()
{

}

void ChunkManager::GenerateChunk(int x, int z)
{
	m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)] = std::make_shared<Chunk>();
	std::lock_guard<std::mutex> lock(m_ChunksMutex);
	m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->SetupLandscape(x, z);
	m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->CreateMesh();
	m_LoadList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)] = m_ChunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)];
	m_ChunkList.erase(glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk));
}

void ChunkManager::AsyncLoadChunks()
{
	for (int x = -centreOfChunks; x <= centreOfChunks; x++)
	{
		for (int z = -centreOfChunks; z <= centreOfChunks; z++)
			m_Futures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, GenerateChunk, x, z)));// [glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)] = std::make_shared<std::future<void>>(std::async(std::launch::async, GenerateChunk, x, z));
	}
}

void ChunkManager::SetupVAO()
{
	/*if (BatchVertexArray.size() == m_AmountOfChunks * m_AmountOfChunks)
	{
		m_Futures.clear();
		return;
	}*/

	for (auto itr = m_LoadList.begin(); itr != m_LoadList.end(); itr++)
	{
		VertexBuffer BatchVertexBuffer(itr->second->GetVertex(), itr->second->GetElementCount() * sizeof * itr->second->GetVertex());
		VertexBufferLayout BatchLayout;
		BatchLayout.Push<GLbyte>(3);
		BatchLayout.Push<GLbyte>(3);
		//BatchLayout.Push<GLbyte>(1);
		BatchVertexArray[itr->first] = std::make_shared<VertexArray>();
		BatchVertexArray[itr->first]->Bind();
		BatchVertexArray[itr->first]->AddBuffer(BatchVertexBuffer, BatchLayout);
		BatchVertexArray[itr->first]->Unbind();
		m_SetupList[itr->first] = itr->second;
	}
}

void ChunkManager::Render(Shader& shader)
{
	for (auto itr = m_SetupList.begin(); itr != m_SetupList.end(); itr++)
	{
		shader.Bind();
		shader.SetUniform3f("u_offset", itr->first.x, 0, itr->first.z);
		itr->second->Render(BatchVertexArray[itr->first], shader);
		m_LoadList.erase(itr->first); //Remove all chunks that have been setup
	}
}

void ChunkManager::Update(Shader& shader)
{
	//Use Camera::GetPosition() in visablityList
	//UpdateAsync();


	UpdateLoadList();

	UpdateUnloadList();

	SetupVAO();

	//UpdateRebuildList();



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
