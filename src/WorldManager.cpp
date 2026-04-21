#include "WorldManager.h"

#include <cstdlib>
#include <iostream>

void WorldManager::updatePosition(glm::vec3 position)
{
    ChunkCoord currentPlayerCoord(position.x, position.z, true);

    // This won't render anything until the player moves on the first frame.
    if(currentPlayerCoord == this->lastFrameCoords)
        return;

    for (int32_t x = -simulationDistance; x <= simulationDistance; ++x) {
        for (int32_t z = -simulationDistance; z <= simulationDistance; ++z) {
            ChunkCoord cc;
            cc.x = currentPlayerCoord.x + x;
            cc.z = currentPlayerCoord.z + z;
            if (this->chunks.find(cc) == chunks.end())
                loadChunk(cc);
        }
    }
    unloadUnneededChunks(currentPlayerCoord);
    lastFrameCoords = currentPlayerCoord;
}

void WorldManager::render(Camera& camera)
{
    shader.use();
    for (auto& [chunkCoord, chunk] : this->chunks) {
        glm::vec3 pos;
        pos.x = chunkCoord.x * ChunkWidth;
        pos.y = 0;
        pos.z = chunkCoord.z * ChunkDepth;

        chunk->mesh.draw(pos, camera, this->shader);
    }
}

void WorldManager::unloadUnneededChunks(ChunkCoord center)
{
    for (auto c = this->chunks.begin(); c != this->chunks.end();) {
        bool inRange = std::abs(c->first.x - center.x) <= simulationDistance &&
                       std::abs(c->first.z - center.z) <= simulationDistance;
        c = inRange ? ++c : this->chunks.erase(c);
    }
}

ChunkNeighbors WorldManager::buildNeighbors(ChunkCoord coord) const
{
    return {
        .north = getChunk({coord.x,     coord.z + 1}),
        .south = getChunk({coord.x,     coord.z - 1}),
        .east  = getChunk({coord.x + 1, coord.z    }),
        .west  = getChunk({coord.x - 1, coord.z    }),
    };
}

void WorldManager::loadChunk(const ChunkCoord& coord)
{
    auto chunk = std::make_unique<Chunk>();
    for (int y = 50; y >= 0; y--)
        chunk->fillLayer(y, BlockType::stone);

    chunk->mesh.createMesh(*chunk, buildNeighbors(coord));
    this->chunks[coord] = std::move(chunk);

    // Remesh already-loaded neighbors — their border faces toward this chunk may now be occluded
    const std::array<ChunkCoord, 4> borderCoords = {{
        ChunkCoord{coord.x,     coord.z + 1},
        ChunkCoord{coord.x,     coord.z - 1},
        ChunkCoord{coord.x + 1, coord.z    },
        ChunkCoord{coord.x - 1, coord.z    },
    }};

    for (const ChunkCoord& nc : borderCoords) {
        if (Chunk* neighbor = getChunk(nc)) {
            neighbor->mesh.createMesh(*neighbor, buildNeighbors(nc));
        }
    }
}

BlockType WorldManager::getBlock(size_t x, size_t y, size_t z) const
{
    BlockType result = BlockType::air;
    //ChunkCoord coord((intx, z);
    //if(auto chunkResult = chunks.find(coord); chunkResult != chunks.end()) {
    //    Chunk& chunk = *chunkResult->second;
    //    // Translate between world coordinates to local coordinates
    //    int localX = x % 16;
    //    int localY = y % 16;
    //    int localZ = z % 16;

    //    result = chunk.getBlock(localX, localY, localZ);
    //}
    return result;
}

Chunk* WorldManager::getChunk(ChunkCoord coord) const
{
    if(auto result = chunks.find(coord); result != chunks.end()) {
        return result->second.get();
    }
    return nullptr;
}