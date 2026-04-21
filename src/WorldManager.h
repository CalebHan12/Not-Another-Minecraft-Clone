#pragma once

#include <cmath>
#include <cstdint>
#include <memory>

#include "Chunk.h"

struct ChunkCoord {
    ChunkCoord() = default;
    // transformBlockCoordiantes - boolean to divide the coordinates by 16 to convert world coordinates
    ChunkCoord(int64_t x, int64_t z, bool transformBlockCoordinates = false) {
        if (transformBlockCoordinates) {
            this->x = x / 16;
            this->z = z / 16;
        } else {
            this->x = x;
            this->z = x;
        }
    }

    ChunkCoord(float x, float z, bool transformBlockCoordinates = false) {
        if (transformBlockCoordinates) {
            this->x = static_cast<int64_t>(std::floor(x / 16.0));
            this->z = static_cast<int64_t>(std::floor(z / 16.0));
        } else {
            this->x = x;
            this->z = x;
        }
    }


    bool operator==(const ChunkCoord& other) const {
        return x == other.x && z == other.z;
    }

    int64_t x;
    int64_t z;
};

struct ChunkCoordHash {
    size_t operator()(const ChunkCoord& coord) const {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.z) << 1);
    }
};

constexpr int simulationDistance = 8;
constexpr int simulationArea = simulationDistance * simulationDistance;

using ChunkMap = std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>;

class WorldManager {
public:
    WorldManager() {
        lastFrameCoords.x = -9999;
        lastFrameCoords.z = -9999;
    }
    ~WorldManager() = default;

    [[nodiscard]] BlockType getBlock(size_t x, size_t y, size_t z) const;
    [[nodiscard]] Chunk* getChunk(ChunkCoord coord) const;

    void updatePosition(glm::vec3 position);
    void render(Camera& camera);

    Shader shader{};
private:
    [[nodiscard]] Chunk getNextChunk(int64_t x, int64_t y, int64_t z) const;
    [[nodiscard]] ChunkNeighbors buildNeighbors(ChunkCoord coord) const;
    void unloadUnneededChunks(ChunkCoord center);
    void loadChunk(const ChunkCoord& coord);

    ChunkCoord lastFrameCoords;
    ChunkMap chunks;
};
