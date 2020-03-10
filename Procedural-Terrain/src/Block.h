#pragma once

#include "GLErrorManager.h"

enum BlockType
{
	BLOCK_NONE  = 0,
	BLOCK_WOOD,
	BLOCK_GRASS,
	BLOCK_DIRT,
	BLOCK_SAND,
	BLOCK_GLASS
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

