#include "ChunkManager.h"
#include "Camera.h"
#include "Timer.h"
#include <condition_variable>
#include <atomic>

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

static std::atomic_bool WindowIsAlive(true);

static glm::ivec3 currentCameraPosition;

ChunkManager::ChunkManager()
{
	m_AmountOfChunks = 33;
	xPosInChunk = 0;
	zPosInChunk = 0;
	centreOfChunks = (m_AmountOfChunks - 1) / 2;

	currentCameraPosition.x = Camera::GetCameraPosition().x;
	currentCameraPosition.z = Camera::GetCameraPosition().z;
}

ChunkManager::~ChunkManager()
{
	WindowIsAlive = false;
	m_ChunkList.clear();
	BatchVertexArray.clear();
	m_Futures.clear();
	m_UpdateFutures.clear();
}

void ChunkManager::UpdateAsync()
{
	
}

void ChunkManager::LoadChunksZDir(int OffsetZ, bool isLoadChunk) //if false delete chunk if true load chunk
{
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunkList;
	for (int x = -centreOfChunks + xPosInChunk; x <= centreOfChunks + xPosInChunk; x++)
	{
		if (!WindowIsAlive) return; //Terminates the thread if the window is closed
		chunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)] = std::make_shared<Chunk>();
		chunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)]->SetupLandscape(x, OffsetZ);
		chunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)]->CreateMesh();
		std::lock_guard<std::mutex> lock(m_LoadMutex);
		m_LoadList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)] = chunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)];
		chunkList.erase(glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk));
	}
}

void ChunkManager::LoadChunksXDir(int OffsetX, bool isLoadChunk)
{
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunkList;
	for (int z = -centreOfChunks + zPosInChunk; z <= centreOfChunks + zPosInChunk; z++)
	{
		if (!WindowIsAlive) return; //Terminates the thread if the window is closed
		chunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)] = std::make_shared<Chunk>();
		chunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)]->SetupLandscape(OffsetX, z);
		chunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)]->CreateMesh();
		std::lock_guard<std::mutex> lock(m_LoadMutex);
		m_LoadList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)] = chunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)];
		chunkList.erase(glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk));
	}
}

void ChunkManager::UpdateLoadList()
{
	int xDir = (Camera::GetCameraPosition().x) - (currentCameraPosition.x);
	int zDir = (Camera::GetCameraPosition().z) - (currentCameraPosition.z);

	if (xDir > SizeOfChunk)
	{
		xPosInChunk++;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksXDir, centreOfChunks + xPosInChunk, true)));
		currentCameraPosition.x += SizeOfChunk;
	}

	if (xDir < 0)
	{
		xPosInChunk--;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksXDir, -centreOfChunks + xPosInChunk, true)));
		currentCameraPosition.x -= SizeOfChunk;
	}

	if (zDir > SizeOfChunk)
	{
		zPosInChunk++;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksZDir, centreOfChunks + zPosInChunk, true)));
		currentCameraPosition.z += SizeOfChunk;
	}

	if (zDir < 0)
	{
		zPosInChunk--;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksZDir, -centreOfChunks + zPosInChunk, true)));
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
			m_SetupList.erase(glm::ivec3((-centreOfChunks + xPosInChunk) * SizeOfChunk, 0, z * SizeOfChunk));
			BatchVertexArray.erase(glm::ivec3((-centreOfChunks + xPosInChunk) * SizeOfChunk, 0, z * SizeOfChunk));
		}
	}

	if (xDir < 0)
	{
		for (int z = -centreOfChunks + zPosInChunk; z <= centreOfChunks + zPosInChunk; z++)
		{
			m_SetupList.erase(glm::ivec3((centreOfChunks + xPosInChunk) * SizeOfChunk, 0, z * SizeOfChunk));
			BatchVertexArray.erase(glm::ivec3((centreOfChunks + xPosInChunk) * SizeOfChunk, 0, z * SizeOfChunk));
		}
	}

	if (zDir > SizeOfChunk)
	{
		for (int x = -centreOfChunks + xPosInChunk; x <= centreOfChunks + xPosInChunk; x++)
		{
			m_SetupList.erase(glm::ivec3(x * SizeOfChunk, 0, (-centreOfChunks + zPosInChunk) * SizeOfChunk));
			BatchVertexArray.erase(glm::ivec3(x * SizeOfChunk, 0, (-centreOfChunks + zPosInChunk) * SizeOfChunk));
		}
	}

	if (zDir < 0)
	{
		for (int x = -centreOfChunks + xPosInChunk; x <= centreOfChunks + xPosInChunk; x++)
		{
			m_SetupList.erase(glm::ivec3(x * SizeOfChunk, 0, (centreOfChunks + zPosInChunk) * SizeOfChunk));
			BatchVertexArray.erase(glm::ivec3(x * SizeOfChunk, 0, (centreOfChunks + zPosInChunk) * SizeOfChunk));
		}
	}
}

void ChunkManager::UpdateRebuildList()
{
	//Update chunks that have been modified e.g. removed Voxel
}

void ChunkManager::UpdateVisibilityList()
{
	//Do frustum culling here
}

void ChunkManager::GenerateChunk()
{

	for (int x = -centreOfChunks; x <= centreOfChunks; x++)
	{
		for (int z = -centreOfChunks; z <= centreOfChunks; z++)
		{
			if (!WindowIsAlive) return; //Terminates the thread if the window is closed
			std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunkList;
			chunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)] = std::make_shared<Chunk>();
			chunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->SetupLandscape(x, z);
			chunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)]->CreateMesh();
			std::lock_guard<std::mutex> lock(m_LoadMutex);
			m_LoadList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)] = chunkList[glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk)];
			chunkList.erase(glm::ivec3(x * SizeOfChunk, 0, z * SizeOfChunk));
		}
	}
}

void ChunkManager::AsyncLoadChunks()
{
	m_Futures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, GenerateChunk)));
}

void ChunkManager::RemoveLoadedList()
{
	std::lock_guard<std::mutex> lock(m_LoadMutex);
	for (auto itr = m_SetupList.begin(); itr != m_SetupList.end(); itr++)
		m_LoadList.erase(itr->first); //Remove all chunks that have been setup
}

void ChunkManager::SetupVAO()
{
	Timer timer("ChunkManager::SetupVAO");
	/*if (BatchVertexArray.size() == m_AmountOfChunks * m_AmountOfChunks)
	{
		m_Futures.clear();
		m_LoadList.clear();
		return;
	}*/

	for (auto itr = m_LoadList.begin(); itr != m_LoadList.end(); itr++)
	{
		if (m_SetupList.find(itr->first) == m_SetupList.end())
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

	m_Futures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, RemoveLoadedList)));
}

void ChunkManager::Render(Shader& shader)
{
	for (auto itr = m_SetupList.begin(); itr != m_SetupList.end(); itr++)
	{
		shader.Bind();
		shader.SetUniform3f("u_offset", itr->first.x, 0, itr->first.z);
		itr->second->Render(BatchVertexArray[itr->first], shader);
	}
}

void ChunkManager::Update(Shader& shader)
{
	//Use Camera::GetPosition() in visablityList
	//UpdateAsync();

	UpdateUnloadList();

	UpdateLoadList();

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
