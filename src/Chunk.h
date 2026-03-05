#pragma once

#include <array>

#include "ChunkMesh.h"

#include "Layer.h"

#include "glm/glm.hpp"

class Camera;
class ChunkMesh;

constexpr uint8_t ChunkSize = 16;
using ChunkLayers = std::array<Layer, ChunkSize>;

class Chunk {
public:
	Chunk() : mesh(this) { };
	Chunk(ChunkLayers chunkLayers);
	~Chunk() = default;

	void buildMesh();
	void draw(glm::vec3& position, Camera& camera);
	[[nodiscard]] BlockType getBlock(uint16_t x, uint16_t y, uint16_t z) const noexcept {
		return this->chunkLayers[z].getBlock(x, y);
	}

	//! Helper function for filling every layer with a specific block
	void fill(BlockType type);
	void setLayer(Layer& l, int z);

private:
	ChunkLayers chunkLayers;
	ChunkMesh mesh;
};
