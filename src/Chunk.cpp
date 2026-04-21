#include "Camera.h"
#include "Chunk.h"
#include "ChunkMesh.h"

void Chunk::fill(BlockType type) {
	blocks.fill(type);
}

void Chunk::fillLayer(int y, BlockType type) {
	if(y >= ChunkHeight || y < 0) { // guard
		return;
	}

	for(size_t z = 0; z < ChunkDepth; ++z) {
		for(size_t x = 0; x < ChunkWidth; ++x) {
			this->setBlock(x, y, z, type);
		}
	}
}

void Chunk::setBlock(int x, int y, int z, BlockType type) {
	size_t index = Chunk::index(x, y, z);
	blocks[index] = type;
}

BlockType Chunk::getBlock(int x, int y, int z) const {
	size_t index = Chunk::index(x, y, z);
	return blocks[index];
}
