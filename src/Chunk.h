#pragma once
#include <array>
#include "glm/glm.hpp"

enum class BlockDirection : uint8_t {
	top = 0,
	bottom,
	front,
	back,
	left,
	right
};

enum class BlockType : uint8_t {
	air = 0,
	grass,
	dirt,
	stone
};

class Camera;
class ChunkMesh;

const uint8_t LayerSize = 16;
using Blocks = std::array<BlockType, LayerSize * LayerSize>;

class Layer {
public:
	Layer() = default;

	Layer(Blocks& blocks) {
		this->blocks = blocks;
	}

	~Layer() = default;

	Layer(const Layer& other) {
		this->blocks = other.blocks;
	}

	Layer& operator=(const Layer& other) {
		this->blocks = other.blocks;
		return *this;
	}

	Layer& operator=(Blocks&& other) {
		this->blocks = other;
		return *this;
	}

	[[nodiscard]] BlockType getBlock(uint16_t x, uint16_t y) const noexcept {
		return blocks[x * y];
	}

	// free this function probably?
	void fill(BlockType type) {
		std::fill(std::begin(blocks), std::end(blocks), type);
	}

private:
	Blocks blocks;
};

constexpr uint8_t ChunkSize = 16;
using ChunkLayers = std::array<Layer, ChunkSize>;

class Chunk {
public:
	Chunk(ChunkLayers chunkLayers);
	~Chunk() = default;
	Chunk(const Chunk& other) = default;
	Chunk& operator=(const Chunk& other) = default;
	Chunk& operator=(Chunk&& other) = default;

	// move this out, shouldn't know about positions or the camera
	// probably need to flesh out player class and world manager
	void draw(glm::vec3& position, Camera& camera);
	[[nodiscard]] BlockType getBlock(uint16_t x, uint16_t y, uint16_t z) const noexcept {
		return this->chunkLayers[z].getBlock(x, y);
	}
	void setLayer(Layer& l, int z);
private:
	size_t x, y, z;
	ChunkLayers chunkLayers;
	ChunkMesh* mesh;
};
