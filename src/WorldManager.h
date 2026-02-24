#pragma once
#include <cstdint>

enum class ChunkState : int8_t {
    Unloaded,
    Generating,
    Meshing,
    Ready,
    Unloading
};

class WorldManager {
};
