#pragma once

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
