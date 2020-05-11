#pragma once
#include "Chunk.h"
#include "glm/gtx/hash.hpp"
#include "Camera.h"
#include "Timer.h"
#include "Frustum.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ChunkManager
{
public:

	ChunkManager(const ChunkManager&) = delete;

	static ChunkManager& GetInstance()
	{
		static ChunkManager instance;

		return instance;
	};

	void Start();

	void Terminate();

	void CreateNewTerrain();

	~ChunkManager();

	void DeleteData();

	static void LoadChunksZDir(int OffsetZ); //Asynchronously generate new chunks in Z direction

	static void LoadChunksXDir(int OffsetX); //Asynchronously generate new chunks in Z direction
	
	static void GenerateChunk(int offsetX, int offsetZ); 

	static void RemoveLoadedList();

	void AsyncLoadChunks(); //Asynchronously generate chunks

	void UpdateLoadList(); //Loads a chunk
	
	void UpdateUnloadList(); //Unloads a chunk
	
	void UpdateVisibilityList(); //Update all the chunks in the list that could be rendered and seen by the camera 

	void SetupVAO(); //Set up the vao, vbo

	void UpdateRenderList(); //Frustum culling, Occlusion culling 

	void Update(Shader& shader); //Updates all the lists

	void Render(Shader& shader);

	int GetPosInChunkX() { return xPosInChunk; }

	int GetPosInChunkZ() { return zPosInChunk; }

private:

	ChunkManager() {};

	static const int SizeOfChunk = 32;

	static int xPosInChunk; //Current x axis position in chunk coordinates
	static int zPosInChunk; //Current z axis position in chunk coordinates

	static std::mutex m_LoadMutex;

	static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_LoadList;
	static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_UnloadList;
	static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_VisibilityList;
	static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_SetupList;
	static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_RenderList;

	static std::unordered_map<glm::ivec3, std::shared_ptr<VertexArray>> BatchVertexArray;

	static int centreOfChunks;

	glm::vec3 currentCameraPosition = glm::vec3(0,0,0);
	static glm::vec3 m_cameraPosition;
	static glm::vec3 m_cameraDirection;

	static std::atomic_bool WindowIsAlive;

	static std::vector<std::shared_ptr<std::future<void>>> m_Futures;
	static std::vector<std::shared_ptr<std::future<void>>> m_UpdateFutures;

	bool firstLoad;
};