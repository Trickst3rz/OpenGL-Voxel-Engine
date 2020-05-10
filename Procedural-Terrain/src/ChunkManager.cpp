#include "ChunkManager.h"
#include "Camera.h"
#include "Timer.h"
#include <condition_variable>
#include <atomic>
#include "Frustum.h"

static std::mutex m_ChunksMutex;
static std::mutex m_UnloadMutex;
static std::mutex m_LoadMutex;
static std::mutex m_CullMutex;
static std::mutex m_RubuildMutex;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_LoadList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_UnloadList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_VisibilityList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_CullList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_SetupList;
static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_RenderList;
static std::unordered_map<glm::ivec3, std::shared_ptr<VertexArray>> BatchVertexArray;

static int m_AmountOfChunks; //Amount of chunks in every direction just a test temp variable atm to see if it works REMOVE LATER ON MAYBE CHANGE TO STATIC?
static int centreOfChunks;
static int xPosInChunk; //Current x axis position in chunk coordinates
static int zPosInChunk; //Current z axis position in chunk coordinates
static int CullListCount = 0;

static std::vector<std::shared_ptr<std::future<void>>> m_Futures;
static std::vector<std::shared_ptr<std::future<void>>> m_UpdateFutures;

static glm::vec3 currentCameraPosition;
static glm::vec3 m_cameraPosition;
static glm::vec3 m_cameraDirection;

static std::atomic_bool WindowIsAlive = true;

static bool firstLoad;

void ChunkManager::Start()
{
	firstLoad = true;
	m_AmountOfChunks = 9;
	xPosInChunk = 0;
	zPosInChunk = 0;
	centreOfChunks = (m_AmountOfChunks - 1) / 2;

	currentCameraPosition.x = Camera::GetCameraPosition().x;
	currentCameraPosition.z = Camera::GetCameraPosition().z;
}

void ChunkManager::Terminate()
{ //Need to terminate before glfwTerminate is called to remove all the buffers
	BatchVertexArray.clear();
}

ChunkManager::~ChunkManager()
{
	WindowIsAlive = false;
	m_Futures.clear();
	m_UpdateFutures.clear();
	m_LoadList.clear();
	m_UnloadList.clear();
	m_SetupList.clear();
	m_VisibilityList.clear();
	m_CullList.clear();
	m_RenderList.clear();
}

void ChunkManager::LoadChunksZDir(int OffsetZ) //if false delete chunk if true load chunk
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

void ChunkManager::LoadChunksXDir(int OffsetX)
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
	auto xDir = (Camera::GetCameraPosition().x) - (currentCameraPosition.x);
	auto zDir = (Camera::GetCameraPosition().z) - (currentCameraPosition.z);

	if (xDir > SizeOfChunk)
	{
		xPosInChunk++;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksXDir, centreOfChunks + xPosInChunk)));
		currentCameraPosition.x += SizeOfChunk;
	}

	if (xDir < 0)
	{
		xPosInChunk--;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksXDir, -centreOfChunks + xPosInChunk)));
		currentCameraPosition.x -= SizeOfChunk;
	}

	if (zDir > SizeOfChunk)
	{
		zPosInChunk++;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksZDir, centreOfChunks + zPosInChunk)));
		currentCameraPosition.z += SizeOfChunk;
	}

	if (zDir < 0)
	{
		zPosInChunk--;
		m_UpdateFutures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, LoadChunksZDir, -centreOfChunks + zPosInChunk)));
		currentCameraPosition.z -= SizeOfChunk;
	}
}

void ChunkManager::UpdateUnloadList()
{//Check if the current position is still inside the same chunk as the new position
	//Make sure that the next chunk is plus the size of the chunk e.g. 32
	auto xDir = Camera::GetCameraPosition().x - currentCameraPosition.x;
	auto zDir = Camera::GetCameraPosition().z - currentCameraPosition.z;
	
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
			m_LoadList.erase(itr->first); //Remove all chunks from the list that have been setup
}

void ChunkManager::CullingChunks(const glm::ivec3& key, std::shared_ptr<Chunk>& value)
{//IF I HAVE TIME DO THE OCCLUSION CULLING BETWEEN TWO CHUNKS, DO IT BY COMPARING TWO NEIGHBOURING CHUNKS
	
	///*std::lock_guard<std::mutex> lock(m_LoadMutex);*/
	//for (auto itr = m_CullList.begin(); itr != m_CullList.end(); itr++)
	//{
	//	m_LoadList.erase(itr->first); //Remove all chunks that have been setup

	//	//Check if chunk to the right exists
	//	if (m_LoadList.count(glm::ivec3(itr->first.x + SizeOfChunk, itr->first.y, itr->first.z)))
	//	{
	//		for (int y = 0; y < SizeOfChunk; y++)
	//		{
	//			for (int z = 0; z < SizeOfChunk; z++)
	//			{
	//				//Cull the occluded vertices
	//				m_SetupList[glm::ivec3(itr->first.x + SizeOfChunk, itr->first.y, itr->first.z)] = m_LoadList[glm::ivec3(itr->first.x + SizeOfChunk, itr->first.y, itr->first.z)];
	//				m_SetupList[glm::ivec3(itr->first.x + SizeOfChunk, itr->first.y, itr->first.z)]->GetBlock(0, y, z)->SetFace(TOP);
	//				m_SetupList[itr->first]->GetBlock(SizeOfChunk - 1, y, z)->SetFace(TOP);
	//				RebuildMesh = true;
	//				CullListCount++;
	//			}
	//		}
	//		m_CullList[glm::ivec3(itr->first.x + SizeOfChunk, itr->first.y, itr->first.z)]->CreateMesh();
	//	}

	//	//Check if chunk to the left exists
	//	if (m_LoadList.count(glm::ivec3(itr->first.x - SizeOfChunk, itr->first.y, itr->first.z)))
	//	{
	//		for (int y = 0; y < SizeOfChunk; y++)
	//		{
	//			for (int z = 0; z < SizeOfChunk; z++)
	//			{
	//				//Cull the occluded vertices
	//				m_SetupList[glm::ivec3(itr->first.x - SizeOfChunk, itr->first.y, itr->first.z)] = m_LoadList[glm::ivec3(itr->first.x - SizeOfChunk, itr->first.y, itr->first.z)];
	//				m_SetupList[glm::ivec3(itr->first.x - SizeOfChunk, itr->first.y, itr->first.z)]->GetBlock(SizeOfChunk - 1, y, z)->SetFace(TOP);
	//				m_SetupList[itr->first]->GetBlock(0, y, z)->SetFace(TOP);
	//				RebuildMesh = true;
	//				CullListCount++;
	//			}
	//		}
	//		m_CullList[glm::ivec3(itr->first.x - SizeOfChunk, itr->first.y, itr->first.z)]->CreateMesh();
	//	}

	//	//Check if chunk forward exists
	//	if (m_LoadList.count(glm::ivec3(itr->first.x, itr->first.y, itr->first.z - SizeOfChunk)))
	//	{
	//		for (int y = 0; y < SizeOfChunk; y++)
	//		{
	//			for (int x = 0; x < SizeOfChunk; x++)
	//			{
	//				//Cull the occluded vertices
	//				m_SetupList[glm::ivec3(itr->first.x, itr->first.y, itr->first.z - SizeOfChunk)] = m_LoadList[glm::ivec3(itr->first.x, itr->first.y, itr->first.z - SizeOfChunk)];
	//				m_SetupList[glm::ivec3(itr->first.x, itr->first.y, itr->first.z - SizeOfChunk)]->GetBlock(x, y, 0)->SetFace(TOP);
	//				m_SetupList[itr->first]->GetBlock(x, y, SizeOfChunk - 1)->SetFace(TOP);
	//				RebuildMesh = true;
	//				CullListCount++;
	//			}
	//		}
	//		m_CullList[glm::ivec3(itr->first.x, itr->first.y, itr->first.z - SizeOfChunk)]->CreateMesh();
	//	}

	//	//Check if chunk behind exists
	//	if (m_LoadList.count(glm::ivec3(itr->first.x, itr->first.y, itr->first.z + SizeOfChunk)))
	//	{
	//		for (int y = 0; y < SizeOfChunk; y++)
	//		{
	//			for (int x = 0; x < SizeOfChunk; x++)
	//			{
	//				//Cull the occluded vertices
	//				m_SetupList[glm::ivec3(itr->first.x, itr->first.y, itr->first.z + SizeOfChunk)] = m_LoadList[glm::ivec3(itr->first.x, itr->first.y, itr->first.z + SizeOfChunk)];
	//				m_SetupList[glm::ivec3(itr->first.x, itr->first.y, itr->first.z + SizeOfChunk)]->GetBlock(x, y, SizeOfChunk - 1)->SetFace(TOP);
	//				m_SetupList[itr->first]->GetBlock(x, y, 0)->SetFace(TOP);


	//				CullListCount++;
	//			}
	//		}
	//		m_SetupList[glm::ivec3(itr->first.x, itr->first.y, itr->first.z + SizeOfChunk)]->CreateMesh();
	//	}
	//}

	//		m_SetupList[itr->first]->CreateMesh();
}



void ChunkManager::UpdateCullingList()
{
	for (auto itr = m_LoadList.begin(); itr != m_LoadList.end(); itr++)
	{
		m_Futures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, CullingChunks, std::ref(itr->first), std::ref(itr->second))));
	}

	//m_Futures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, RemoveLoadedList, std::ref(m_LoadList), std::ref(m_LoadMutex))));
}

void ChunkManager::SetupVAO()
{
	/*if (BatchVertexArray.size() == m_AmountOfChunks * m_AmountOfChunks)
	{
		m_Futures.clear();
		m_LoadList.clear();
		firstLoad = false;
		return;
	}*/

	for (auto itr = m_LoadList.begin(); itr != m_LoadList.end(); itr++)
	{
		VertexBuffer BatchVertexBuffer(itr->second->GetVertex(), itr->second->GetElementCount() * sizeof * itr->second->GetVertex());
		VertexBufferLayout BatchLayout;
		BatchLayout.Push<GLbyte>(3); //Position layout
		BatchLayout.Push<GLbyte>(3); //Normal layout
		BatchLayout.Push<GLubyte>(3); //Colour layout
		BatchVertexArray[itr->first] = std::make_shared<VertexArray>();
		BatchVertexArray[itr->first]->Bind();
		BatchVertexArray[itr->first]->AddBuffer(BatchVertexBuffer, BatchLayout);
		BatchVertexArray[itr->first]->Unbind();
		m_SetupList[itr->first] = itr->second;
	}

	m_Futures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, RemoveLoadedList)));
}

void ChunkManager::UpdateVisibilityList()
{
	m_VisibilityList.clear();

	for (auto itr = m_SetupList.begin(); itr != m_SetupList.end(); itr++)
	{
		if (Frustum::GetInstance().CubeInFrustum(itr->first, SizeOfChunk) > 0 || !Global::ToggleFrustum && !Global::FrustumCamera)
		{
			m_VisibilityList[itr->first] = itr->second;
		}
	}
}
void ChunkManager::UpdateRenderList()
{
	//Clear render list each frame so the chunks that can be seen are only rendered
	m_RenderList.clear();

	//Toggle for auto frustum culling
	if (Global::ToggleFrustum)
	{
		Frustum::GetInstance().SetCamera(Camera::GetCameraPosition(), Camera::GetCameraFront(), Camera::GetCameraUp(), Camera::GetCameraRight());
		Global::FrustumCamera = false;
	}

	for (auto itr = m_VisibilityList.begin(); itr != m_VisibilityList.end(); itr++)
	{
		m_RenderList[itr->first] = itr->second;
	}
}

void ChunkManager::Render(Shader& shader)
{

	for (auto itr = m_RenderList.begin(); itr != m_RenderList.end(); itr++)
	{
		shader.Bind();
		shader.SetUniform3f("u_offset", (float)itr->first.x, 0, (float)itr->first.z);
		itr->second->Render(BatchVertexArray[itr->first], shader);
	}
}

void ChunkManager::Update(Shader& shader)
{
	UpdateUnloadList();

	UpdateLoadList();

	//UpdateCullingList();

	SetupVAO();

	UpdateVisibilityList();

	if (m_cameraPosition != Camera::GetCameraPosition() || m_cameraDirection != Camera::GetCameraFront() || firstLoad)
	{
		UpdateRenderList();
	}
	m_cameraPosition = Camera::GetCameraPosition();
	m_cameraDirection = Camera::GetCameraFront();

	Render(shader);
}

void ChunkManager::SetChunkDistance(int numOfChunks)
{//Keep amount of chunks as a const value for now
	//Maybe add a button to ImGui that you click to load the terrain again

	m_AmountOfChunks = numOfChunks + 1;

	//SetupVAO();
}
