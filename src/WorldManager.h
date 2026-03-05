#pragma once

#include <cstdint>
#include <memory>
#include <unordered_set>

#include "Chunk.h"

enum class ChunkState : int8_t {
    Unloaded = 0,
    Generating,
    Meshing,
    Ready,
    Unloading
};

struct ChunkCoord {
    ChunkCoord() = default;
    explicit ChunkCoord(double x, double z) {
        this->x = static_cast<int32_t>(floor(x / ChunkSize));
        this->z = static_cast<int32_t>(float(z / ChunkSize));
    }

    bool operator==(const ChunkCoord& other) const {
        return x == other.x && z == other.z;
    }

    int32_t x;
    int32_t z;
};

struct ChunkCoordHash {
    size_t operator()(const ChunkCoord& coord) const {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.z) << 1);
    }
};

constexpr int simulationDistance = 15;
constexpr int simulationArea = simulationDistance * simulationDistance;

using ChunkMap = std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>;

class WorldManager {
public:
    WorldManager() = default;
    ~WorldManager() = default;

    void updatePosition(glm::vec3 position);
    void unloadUnneededChunks(const std::unordered_set<ChunkCoord, ChunkCoordHash>& neededChunks);
    void render() const;

    ChunkCoord lastFrameCoords;
private:
    [[nodiscard]] Chunk getNextChunk(int64_t x, int64_t y, int64_t z) const;
    void loadChunk(const ChunkCoord& cord);

    ChunkMap chunks;

};
