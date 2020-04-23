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
	
	void UpdateLoadList(); //Loads a chunk
	
	void UpdateUnloadList(); //Unloads a chunk
	
	void UpdateRebuildList(); //Updates the mesh when a block is removed or added to a chunk
	
	void UpdateVisibilityList(); //ADD CAMERA PARAMETER OR MAKE CAMERA A STATIC CLASS
	//Update all the chunks in the list that could be rendered and seen by the camera 

	static void GenerateChunk(std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>>& chunkList, std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>>& LoadList, int x, int z);

	void AsyncLoadChunks();

	void SetupVAO(float deltaTime); //Set up the vao, vbo

	void Render(Shader& shader); //Frustum culling, Occlusion culling 

	void Update(Shader& shader); //Updates all the lists

	void SetChunkDistance(int numOfChunks);


private:
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_ChunkList;
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_LoadList;
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_UnloadList;
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_RebuildList;
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_VisibilityList;
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_SetupList;
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_RenderList;

	std::vector<std::future<void>> m_Futures;

	int m_AmountOfChunks; //Amount of chunks in every direction just a test temp variable atm to see if it works REMOVE LATER ON MAYBE CHANGE TO STATIC?
	int centreOfChunks;
	int xPosInChunk; //Current x axis position in chunk coordinates
	int zPosInChunk; //Current z axis position in chunk coordinates

	glm::ivec3 currentCameraPosition;

	static const int SizeOfChunk = 32;

	//OpenGL
	std::unordered_map<glm::ivec3, std::shared_ptr<VertexArray>> BatchVertexArray;
};