#include "Block.h"

Block::Block() : m_blockActive(false), m_blockType(BLOCK_DEEP_WATER)
{
	m_face[TOP] = TOP;
	m_face[BOTTOM] = BOTTOM;
	m_face[LEFT] = LEFT;
	m_face[RIGHT] = RIGHT;
	m_face[FRONT] = FRONT;
	m_face[BACK] = BACK;
}

Block::~Block()
{

}
