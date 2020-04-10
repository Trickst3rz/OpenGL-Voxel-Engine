#pragma once
#include "Chunk.h"

class ChunkManager
{
public:

	ChunkManager(int x, int z);

	~ChunkManager();

	void UpdateAsync(); //Future asynchronous of loading chunks

	void UpdateLoadList(); //Loads a chunk

	void UpdateUnloadList(); //Unloads a chunk

	void UpdateRebuildList(); //Updates the mesh when a block is removed or added to a chunk

	void UpdateVisibilityList(); //ADD CAMERA PARAMETER OR MAKE CAMERA A STATIC CLASS
	//Update all the chunks in the list that could be rendered and seen by the camera 

	void Render(); //Frustum culling, Occlusion culling 

	void Update(); //Updates all the lists

private:
	Chunk** chunks;
};