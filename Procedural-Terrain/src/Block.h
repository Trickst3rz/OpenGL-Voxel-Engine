#pragma once

#include "GLErrorManager.h"

enum BlockType
{
	BLOCK_DEEP_WATER = 0,
	BLOCK_SHALLOW_WATER,
	BLOCK_SHORE,
	BLOCK_SAND,
	BLOCK_GRASS,
	BLOCK_DIRT,
	BLOCK_ROCK,
	BLOCK_SNOW
};

class Block
{
public:

	Block();

	~Block();

	bool isActive() { return m_blockActive; }

	void SetActive(bool setActive) { m_blockActive = setActive; }

	BlockType GetType() { return m_blockType; }

	void SetType(BlockType type) { m_blockType = type; }


private:

	bool m_blockActive;
	BlockType m_blockType;
};

