#pragma once

#include <array>

#include "Blocks.h"
#include "ChunkMesh.h"

#include "glm/glm.hpp"


constexpr int ChunkWidth = 16;
constexpr int ChunkHeight = 256;
constexpr int ChunkDepth = 16;
constexpr int TotalChunkBlocks = ChunkWidth * ChunkHeight * ChunkDepth;

class Camera;

class Chunk {
public:
	Chunk() { blocks.fill(BlockType::air); };
	~Chunk() = default;

	static int index(int x, int y, int z) { return x + ChunkWidth * y + (ChunkWidth * ChunkHeight) * z; }

	//! Helper function for filling every layer with a specific block
	void fill(BlockType type);
	//! Fill a single y level with a specific block
	void fillLayer(int y, BlockType type);
	void setBlock(int x, int y, int z, BlockType type);

	BlockType getBlock(int x, int y, int z) const;

	ChunkMesh mesh;
private:
	std::array<BlockType, TotalChunkBlocks> blocks;
};
