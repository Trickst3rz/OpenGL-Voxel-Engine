#pragma once

#include "GLErrorManager.h"

enum BlockType
{
	BLOCK_NONE  = 0,
	BLOCK_WOOD  = 1,
	BLOCK_GRASS = 2,
	BLOCK_DIRT  = 3,
	BLOCK_SAND  = 4,
	BLOCK_GLASS = 5
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

