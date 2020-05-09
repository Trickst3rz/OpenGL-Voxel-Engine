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

	ChunkManager();

	~ChunkManager();

	void UpdateAsync(); //Future asynchronous of loading chunks

	static void LoadChunksZDir(int OffsetZ, bool isLoadChunk);

	static void LoadChunksXDir(int OffsetX, bool isLoadChunk);
	
	static void UpdateLoadList(); //Loads a chunk
	
	static void UpdateUnloadList(); //Unloads a chunk
	
	static void UpdateRebuildList(); //Updates the mesh when a block is removed or added to a chunk
	
	static void UpdateVisibilityList(); //ADD CAMERA PARAMETER OR MAKE CAMERA A STATIC CLASS
	//Update all the chunks in the list that could be rendered and seen by the camera 

	static void GenerateChunk();

	static void RemoveLoadedList();

	static void AsyncLoadChunks();

	static void SetupVAO(); //Set up the vao, vbo

	static void UpdateRenderList(); //Frustum culling, Occlusion culling 

	static void Render(Shader& shader);

	static void Update(Shader& shader); //Updates all the lists

	static void SetChunkDistance(int numOfChunks);

private:

	static const int SizeOfChunk = 32;
};