#include "ChunkManager.h"

std::mutex ChunkManager::m_LoadMutex;

std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> ChunkManager::m_LoadList;
std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> ChunkManager::m_UnloadList;
std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> ChunkManager::m_VisibilityList;
std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> ChunkManager::m_SetupList;
std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> ChunkManager::m_RenderList;

std::unordered_map<glm::ivec3, std::shared_ptr<VertexArray>> ChunkManager::BatchVertexArray;

int ChunkManager::centreOfChunks;

int ChunkManager::xPosInChunk = 0;
int ChunkManager::zPosInChunk = 0;

glm::vec3 ChunkManager::m_cameraPosition;
glm::vec3 ChunkManager::m_cameraDirection;

std::atomic_bool ChunkManager::WindowIsAlive;

std::vector<std::shared_ptr<std::future<void>>> ChunkManager::m_Futures;
std::vector<std::shared_ptr<std::future<void>>> ChunkManager::m_UpdateFutures;

void ChunkManager::Start()
{
	firstLoad = true;
	WindowIsAlive = true;
	centreOfChunks = (Global::GetInstance().GetAmountOfChunks() - 1) / 2;
}

void ChunkManager::Terminate()
{	//Need to terminate before glfwTerminate is called to remove all the buffers
	BatchVertexArray.clear();
}

void ChunkManager::CreateNewTerrain()
{
	Terminate();
	DeleteData();
	Start();
	AsyncLoadChunks();
}

ChunkManager::~ChunkManager()
{
	DeleteData();
}

void ChunkManager::DeleteData()
{
	WindowIsAlive = false;
	m_Futures.clear();
	m_UpdateFutures.clear();
	m_LoadList.clear();
	m_UnloadList.clear();
	m_SetupList.clear();
	m_VisibilityList.clear();
	m_RenderList.clear();
}

void ChunkManager::LoadChunksZDir(int OffsetZ)
{
	for (int x = -centreOfChunks + xPosInChunk; x <= centreOfChunks + xPosInChunk; x++)
	{//On another thread setup and create the mesh then add it to loadList when finished
		if (!WindowIsAlive) return; //Terminates the thread if the window is closed
		std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunkList;
		chunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)] = std::make_shared<Chunk>();
		chunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)]->SetupLandscape(x, OffsetZ);
		chunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)]->CreateMesh();
		std::lock_guard<std::mutex> lock(m_LoadMutex); //Lock so no other threads can access m_LoadList while this thread accesses it
		m_LoadList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)] = chunkList[glm::ivec3(x * SizeOfChunk, 0, OffsetZ * SizeOfChunk)];
		chunkList.clear();
	}
}

void ChunkManager::LoadChunksXDir(int OffsetX)
{
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunkList;
	for (int z = -centreOfChunks + zPosInChunk; z <= centreOfChunks + zPosInChunk; z++)
	{//On another thread setup and create the mesh then add it to loadList when finished
		if (!WindowIsAlive) return; //Terminates the thread if the window is closed
		chunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)] = std::make_shared<Chunk>();
		chunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)]->SetupLandscape(OffsetX, z);
		chunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)]->CreateMesh();
		std::lock_guard<std::mutex> lock(m_LoadMutex); //Lock so no other threads can access m_LoadList while this thread accesses it
		m_LoadList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)] = chunkList[glm::ivec3(OffsetX * SizeOfChunk, 0, z * SizeOfChunk)];
		chunkList.clear();
	}
}

void ChunkManager::UpdateLoadList()
{	//Check if the current position is still inside the same chunk as the new position
	//Make sure that the next chunk is plus the size of the chunk e.g. 32
	//Check the x, z position, if its greater than chunk size or less than 0 meaning its inside a new chunk so load chunks that are in front
	//If true increment xPosInChunk or zPosInChunk to keep track of which chunk the player is in e.g. 0,0 starting position, 1,0 chunk to the right of start position
	auto xDir = (Camera::GetInstance().GetCameraPosition().x) - (currentCameraPosition.x);
	auto zDir = (Camera::GetInstance().GetCameraPosition().z) - (currentCameraPosition.z);

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
{	//Check if the current position is still inside the same chunk as the new position
	//Make sure that the next chunk is plus the size of the chunk e.g. 32
	//Check the x, z position, if its greater than chunk size or less than 0 meaning its inside a new chunk so unload chunks that are behind
	auto xDir = Camera::GetInstance().GetCameraPosition().x - currentCameraPosition.x;
	auto zDir = Camera::GetInstance().GetCameraPosition().z - currentCameraPosition.z;
	
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


void ChunkManager::GenerateChunk(int offsetX, int offsetZ)
{
	for (int x = -centreOfChunks; x <= centreOfChunks; x++)
	{
		for (int z = -centreOfChunks; z <= centreOfChunks; z++)
		{ //On another thread setup and create the mesh then add it to loadList when finished
			if (!WindowIsAlive) return; //Terminates the thread if the window is closed
			std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunkList;
			chunkList[glm::ivec3((x + offsetX) * SizeOfChunk, 0, (z + offsetZ) * SizeOfChunk)] = std::make_shared<Chunk>();
			chunkList[glm::ivec3((x + offsetX) * SizeOfChunk, 0, (z + offsetZ) * SizeOfChunk)]->SetupLandscape(x + (double)offsetX, z + (double)offsetZ);
			chunkList[glm::ivec3((x + offsetX) * SizeOfChunk, 0, (z + offsetZ) * SizeOfChunk)]->CreateMesh();
			std::lock_guard<std::mutex> lock(m_LoadMutex); //Lock so no other threads can access m_LoadList while this thread accesses it
			m_LoadList[glm::ivec3((x + offsetX) * SizeOfChunk, 0, (z + offsetZ) * SizeOfChunk)] = chunkList[glm::ivec3((x + offsetX) * SizeOfChunk, 0, (z + offsetZ) * SizeOfChunk)];
			chunkList.clear();
		}
	}
}

void ChunkManager::AsyncLoadChunks()
{
	m_Futures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, GenerateChunk, xPosInChunk, zPosInChunk)));
}

void ChunkManager::RemoveLoadedList()
{
	std::lock_guard<std::mutex> lock(m_LoadMutex); //Lock so no other threads can access m_LoadList while this thread accesses it
	for (auto itr = m_SetupList.begin(); itr != m_SetupList.end(); itr++)
			m_LoadList.erase(itr->first); //Remove all chunks from the list that have been setup
}

void ChunkManager::SetupVAO()
{	//Early exit if all chunks are setup
	if (BatchVertexArray.size() == (double)Global::GetInstance().GetAmountOfChunks() * (double)Global::GetInstance().GetAmountOfChunks())
	{
		firstLoad = false;
		return;
	}

	for (auto itr = m_LoadList.begin(); itr != m_LoadList.end(); itr++)
	{	//From the loadlist Setup the chunk e.g. create VBO, layout and bind it to a VAO
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
	//Clear loadlist after setup so no unnecessary chunks are setup when they already exist
	m_Futures.push_back(std::make_shared<std::future<void>>(std::async(std::launch::async, RemoveLoadedList)));
}

void ChunkManager::UpdateVisibilityList()
{
	m_VisibilityList.clear();

	//Add all chunks to the list that are visual to the camera
	for (auto itr = m_SetupList.begin(); itr != m_SetupList.end(); itr++)
	{
		if (Frustum::GetInstance().CubeInFrustum(itr->first, SizeOfChunk) > 0 || !Global::GetInstance().ToggleFrustum && !Global::GetInstance().FrustumCamera)
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
	if (Global::GetInstance().ToggleFrustum)
	{
		Frustum::GetInstance().SetCamera(Camera::GetInstance().GetCameraPosition(), Camera::GetInstance().GetCameraFront(), Camera::GetInstance().GetCameraUp());
		Global::GetInstance().FrustumCamera = false;
	}

	//Add all the visual chunks to the renderlist so its ready to be rendered
	for (auto itr = m_VisibilityList.begin(); itr != m_VisibilityList.end(); itr++)
	{
		m_RenderList[itr->first] = itr->second;
	}
}

void ChunkManager::Render(Shader& shader)
{

	for (auto itr = m_RenderList.begin(); itr != m_RenderList.end(); itr++)
	{ //Render each chunk and set its offset accordingly
		shader.Bind();
		shader.SetUniform3f("u_offset", (float)itr->first.x, 0, (float)itr->first.z);
		itr->second->Render(BatchVertexArray[itr->first], shader);
	}
}

void ChunkManager::Update(Shader& shader)
{
	UpdateUnloadList();

	UpdateLoadList();

	SetupVAO();

	UpdateVisibilityList();

	if (m_cameraPosition != Camera::GetInstance().GetCameraPosition() || m_cameraDirection != Camera::GetInstance().GetCameraFront() || firstLoad)
	{
		UpdateRenderList();
	}
	m_cameraPosition = Camera::GetInstance().GetCameraPosition();
	m_cameraDirection = Camera::GetInstance().GetCameraFront();

	Render(shader);
}
