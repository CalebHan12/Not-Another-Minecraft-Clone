#include "WorldManager.h"

#include <unordered_set>
#include <iostream>

//! Quick chunk generator for now...
//! Returns a whole dirt chunk for all chunks under y-level 15 and air for all above
Chunk WorldManager::getNextChunk(int64_t x, int64_t y, int64_t z) const
{
    (void)x;
    (void)z;
    Chunk result;
    if (y < 16)
        result.fill(BlockType::dirt);
    else
        result.fill(BlockType::air);
    return result;
}

void WorldManager::updatePosition(glm::vec3 position)
{
    ChunkCoord currentPlayerCoord(position.x, position.z);

    // TODO: This won't render anything until the player moves on the first frame.
    if(currentPlayerCoord == this->lastFrameCoords)
        return;

    // A list of chunks we need for the current position
    // We'll use this later to look at the current loaded chunks (this->chunks) to determine
    // what needs to be thrown out
    std::unordered_set<ChunkCoord, ChunkCoordHash> neededChunks;

    if(loadOnce) {
        for (int32_t x = -simulationDistance; x <= simulationDistance; ++x) {
            for (int32_t z = -simulationDistance; z <= simulationDistance; ++z) {
                ChunkCoord cc;
                cc.x = currentPlayerCoord.x + x;
                cc.z = currentPlayerCoord.z + z;
                neededChunks.insert(cc);
                if (this->chunks.find(cc) == chunks.end()) {
                    std::cout << "Loading chunk " << x << " " << z << "\n";
                    loadChunk(cc);
                }
            }
        }
        unloadUnneededChunks(neededChunks);
        loadOnce = false;
    }
}

void WorldManager::render(Camera& camera)
{
    shader.use();
    for (auto& [chunkCoord, chunk] : this->chunks) {
        glm::vec3 pos;
        pos.x = chunkCoord.x * 16;
        pos.y = 0;
        pos.z = chunkCoord.z * 16;

        chunk->mesh.draw(pos, camera, this->shader);
    }
}

void WorldManager::unloadUnneededChunks(const std::unordered_set<ChunkCoord, ChunkCoordHash>& neededChunks)
{
    // TODO: Replace this with a quick check for if the chunk coordinates are within the (player position) + render distance
    for (auto c = this->chunks.begin(); c != this->chunks.end();) {
        if (neededChunks.find(c->first) == neededChunks.end())
            c = this->chunks.erase(c);
        else
            ++c;
    }
}

void WorldManager::loadChunk(const ChunkCoord &coord)
{
    std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>();
    chunk->fill(BlockType::air);
    for(int y = 50; y >= 0; y--) {
        chunk->fillLayer(y, BlockType::dirt);
    }
    chunk->mesh.createMesh(*chunk);

    this->chunks[coord] = std::move(chunk);
}

BlockType WorldManager::getBlock(size_t x, size_t y, size_t z) const
{
    BlockType result = BlockType::air;
    ChunkCoord coord(x, z);
    if(auto chunkResult = chunks.find(coord); chunkResult != chunks.end()) {
        Chunk& chunk = *chunkResult->second;
        // Translate between world coordinates to local coordinates
        int localX = x % 16;
        int localY = y % 16;
        int localZ = y % 16;

        result = chunk.getBlock(localX, localY, localZ);
    }
    return result;
}

Chunk* WorldManager::getChunk(ChunkCoord coord) const
{
    if(auto result = chunks.find(coord); result != chunks.end()) {
        return result->second.get();
    }
    return nullptr;
}