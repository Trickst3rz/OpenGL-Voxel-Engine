#include "Block.h"

Block::Block() : m_blockActive(true), m_blockType(BLOCK_NONE)
{

}

Block::~Block()
{
	std::cout << "Deleting block.." << std::endl;
}
