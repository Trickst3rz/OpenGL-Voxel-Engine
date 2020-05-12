#pragma once
/*Enums are unsigned char since we don't need more than 0 - 255 and it makes each block very small,
so each block is just 8 bytes compared to before 28 bytes with normal 4 byte enums.
*/


enum BlockType : unsigned char
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
enum RenderFace : unsigned char 
{
	TOP = 0,
	BOTTOM,
	LEFT,
	RIGHT,
	FRONT,
	BACK,
	NONE //If set to none then don't render that face
};

class Block
{
private:
public:

	Block();

	~Block();

	bool isActive() { return m_blockActive; }

	void SetActive(bool setActive) { m_blockActive = setActive; }

	BlockType GetType() { return m_blockType; }

	void SetType(BlockType type) { m_blockType = type; }

	RenderFace GetFace(RenderFace index) { return m_face[index]; }

	void SetFace(RenderFace index, RenderFace setFace) { m_face[index] = setFace; }

private:
	bool m_blockActive;
	BlockType m_blockType;
	RenderFace m_face[6];
};

