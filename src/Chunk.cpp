#include "Camera.h"
#include "Chunk.h"
#include "ChunkMesh.h"

Chunk::Chunk(ChunkLayers chunkLayers) : mesh{this}
{
	this->chunkLayers = chunkLayers;
	this->mesh.createMesh();
}

void Chunk::draw(glm::vec3& position, Camera& camera)
{
	this->mesh.draw(position, camera);
}

void Chunk::buildMesh()
{
	this->mesh.createMesh();
}

void Chunk::fill(BlockType type) {
	for(Layer& l : this->chunkLayers) {
		l.fill(type);
	}
}

void Chunk::setLayer(Layer& l, int z) {
	if(0 < z && z < ChunkSize) {
		this->chunkLayers[z] = l;
	}
}