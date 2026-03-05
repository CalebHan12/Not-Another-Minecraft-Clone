#pragma once

#include <array>
#include <stdint.h>

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

const uint8_t LayerSize = 16;
using Blocks = std::array<BlockType, LayerSize * LayerSize>;

class Layer {
public:
	Layer() = default;

	Layer(Blocks& blocks) {
		this->blocks = blocks;
	}

	~Layer() = default;
	Layer(const Layer& other) = default;
	Layer(Layer&& other) noexcept = default;

	Layer& operator=(const Layer& other) noexcept = default;
	Layer& operator=(Blocks&& other) noexcept {
		this->blocks = other;
		return *this;
	}

	[[nodiscard]] BlockType getBlock(uint16_t x, uint16_t y) const noexcept {
		return blocks[x * y];
	}

	void fill(BlockType type) {
		//std::fill(std::begin(blocks), std::end(blocks), type);
		blocks.fill(type);
	}

private:
	Blocks blocks;
};
