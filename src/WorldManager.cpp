#include "WorldManager.h"

#include <unordered_set>
#include <iostream>

//! Quick chunk generator for now...
//! Returns a whole dirt chunk for all chunks under y-level 15 and air for all above
Chunk WorldManager::getNextChunk(int64_t x, int64_t y, int64_t z) const
{
    Chunk result;
    if (y < 16)
        result.fill(BlockType::dirt);
    else
        result.fill(BlockType::air);
    return result;
}

void WorldManager::updatePosition(glm::vec3 position)
{
    std::cout << "Camera X: " << position.x << "\n";
    std::cout << "Camera Z: " << position.z << "\n";
    ChunkCoord currentPlayerCoord(position.x, position.z);

    // TODO: This won't render anything until the player moves on the first frame.
    if(currentPlayerCoord == this->lastFrameCoords)
        return;

    // A list of chunks we need for the current position
    // We'll use this later to look at the current loaded chunks (this->chunks) to determine
    // what needs to be thrown out
    std::unordered_set<ChunkCoord, ChunkCoordHash> neededChunks;

    for (int32_t x = -simulationDistance; x <= simulationDistance; ++x) {
        for (int32_t z = -simulationDistance; z <= simulationDistance; ++z) {
            ChunkCoord cc;
            cc.x = currentPlayerCoord.x + x;
            cc.z = currentPlayerCoord.z + z;
            neededChunks.insert(cc);
            if (this->chunks.find(cc) == chunks.end())
            {
                loadChunk(cc);
            }
        }
    }
    unloadUnneededChunks(neededChunks);
}

void WorldManager::render(Camera& camera)
{
    for (auto& [chunkCoord, chunk] : this->chunks)
    {
        glm::vec3 pos;
        pos.x = chunkCoord.x * 16;
        pos.y = 0;
        pos.z = chunkCoord.z * 16;

        chunk->draw(pos, camera);
    }
}

void WorldManager::unloadUnneededChunks(const std::unordered_set<ChunkCoord, ChunkCoordHash>& neededChunks)
{
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
    chunk->fill(BlockType::grass);
    chunk->buildMesh();

    this->chunks[coord] = std::move(chunk);
}