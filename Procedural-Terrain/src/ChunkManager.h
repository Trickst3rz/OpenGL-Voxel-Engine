#pragma once
#include "Chunk.h"
#include "Shader.h"
#include <vector>
#include <unordered_map>
#include "glm/gtx/hash.hpp"
#include <memory>
#include <future>
#include <mutex>

class ChunkManager
{
public:

	ChunkManager(const ChunkManager&) = delete;

	static ChunkManager& GetInstance()
	{
		static ChunkManager instance;

		return instance;
	};

	static void Start();

	~ChunkManager();

	static void LoadChunksZDir(int OffsetZ);

	static void LoadChunksXDir(int OffsetX);
	
	static void GenerateChunk();

	static void RemoveLoadedList();

	static void AsyncLoadChunks();

	void UpdateLoadList(); //Loads a chunk
	
	void UpdateUnloadList(); //Unloads a chunk
	
	void UpdateVisibilityList(); //ADD CAMERA PARAMETER OR MAKE CAMERA A STATIC CLASS
	//Update all the chunks in the list that could be rendered and seen by the camera 

	static void CullingChunks(const glm::ivec3& key, std::shared_ptr<Chunk>& value);

	static void UpdateCullingList(); //Cull occluded vertices between chunks

	void SetupVAO(); //Set up the vao, vbo

	void UpdateRenderList(); //Frustum culling, Occlusion culling 

	void Update(Shader& shader); //Updates all the lists

	void Render(Shader& shader);

	void SetChunkDistance(int numOfChunks);

private:

	ChunkManager() {};

	static const int SizeOfChunk = 32;
};