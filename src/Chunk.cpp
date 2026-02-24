#include "Camera.h"
#include "Chunk.h"
#include "ChunkMesh.h"

Chunk::Chunk(ChunkLayers chunkLayers)
{
	this->chunkLayers = chunkLayers;
	this->mesh = new ChunkMesh(this);
	this->mesh->createMesh();
}

void Chunk::draw(glm::vec3& position, Camera& camera)
{
	this->mesh->draw(position, camera);
}
