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

	static ChunkManager& GetInstance()
	{
		ChunkManager instance;
		return instance;
	}

	ChunkManager();

	~ChunkManager();

	void UpdateAsync(); //Future asynchronous of loading chunks

	static void LoadChunksZDir(int OffsetZ, bool isLoadChunk);

	static void LoadChunksXDir(int OffsetX, bool isLoadChunk);
	
	static void UpdateLoadList(); //Loads a chunk
	
	static void UpdateUnloadList(); //Unloads a chunk
	
	static void UpdateRebuildList(); //Updates the mesh when a block is removed or added to a chunk
	
	void UpdateVisibilityList(); //ADD CAMERA PARAMETER OR MAKE CAMERA A STATIC CLASS
	//Update all the chunks in the list that could be rendered and seen by the camera 

	static void GenerateChunk();

	static void RemoveLoadedList();

	void AsyncLoadChunks();

	void SetupVAO(); //Set up the vao, vbo

	void Render(Shader& shader); //Frustum culling, Occlusion culling 

	void Update(Shader& shader); //Updates all the lists

	void SetChunkDistance(int numOfChunks);

private:

	static const int SizeOfChunk = 32;
};