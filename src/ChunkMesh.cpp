#include "ChunkMesh.h"
#include "Game.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/matrix.hpp"

#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

// Face drawing order
// Bottom left, top left, top right, bottom right

constexpr std::array<GLfloat, 12> frontFace = {
	0, 0, 1,
	0, 1, 1,
	1, 1, 1,
	1, 0, 1
};

constexpr std::array<GLfloat, 12> backFace = {
	0, 0, 0,
	0, 1, 0,
	1, 1, 0,
	1, 0, 0
};

constexpr std::array<GLfloat, 12> topFace = {
	0, 1, 1,
	0, 1, 0,
	1, 1, 0,
	1, 1, 1
};

constexpr std::array<GLfloat, 12> bottomFace = {
	0, 0, 1,
	0, 0, 0,
	1, 0, 0,
	1, 0, 1
};

constexpr std::array<GLfloat, 12> leftFace = {
	0, 0, 0,
	0, 1, 0,
	0, 1, 1,
	0, 0, 1
};

constexpr std::array<GLfloat, 12> rightFace = {
	1, 0, 1,
	1, 1, 1,
	1, 1, 0,
	1, 0, 0
};

static glm::ivec3 getAdjacentCoords(int x, int y, int z, BlockDirection direction) {
	switch (direction) {
		case BlockDirection::top:    return { x, y + 1, z };
		case BlockDirection::bottom: return { x, y - 1, z };
		case BlockDirection::front:  return { x, y, z + 1 };
		case BlockDirection::back:   return { x, y, z - 1 };
		case BlockDirection::left:   return { x - 1, y, z };
		case BlockDirection::right:  return { x + 1, y, z };
		default:                     return { x, y, z };
	}
}

ChunkMesh::~ChunkMesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void ChunkMesh::draw(const glm::vec3& position, Camera& camera, Shader& shader) const {
	// Uncomment to enable wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    float angle = 0.0f;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    shader.setMat4("model", model);

    Game::texture->useTexture();

	glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indiciesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ChunkMesh::createMesh(const Chunk& chunk, ChunkNeighbors neighbors)
{
	vertices.clear();
	indices.clear();
	constexpr size_t maxFaces = ChunkWidth * ChunkHeight * ChunkDepth * 6;
	vertices.reserve(maxFaces * 4 * 5);
	indices.reserve(maxFaces * 6);
	for (int z = 0; z < ChunkDepth; ++z) {
		for (int y = 0; y < ChunkHeight; ++y) {
			for (int x = 0; x < ChunkWidth; ++x) {
				BlockType blockType = chunk.getBlock(x, y, z);
				if(blockType == BlockType::air)
					continue;

				if(!doesFaceExist(chunk, neighbors, BlockDirection::front, x, y, z))
					addFaceToMesh(blockType, BlockDirection::front, frontFace, x, y, z);

				if(!doesFaceExist(chunk, neighbors, BlockDirection::back, x, y, z))
					addFaceToMesh(blockType, BlockDirection::back, backFace, x, y, z);

				if(!doesFaceExist(chunk, neighbors, BlockDirection::top, x, y, z))
					addFaceToMesh(blockType, BlockDirection::top, topFace, x, y, z);

				if(!doesFaceExist(chunk, neighbors, BlockDirection::bottom, x, y, z))
					addFaceToMesh(blockType, BlockDirection::bottom, bottomFace, x, y, z);

				if(!doesFaceExist(chunk, neighbors, BlockDirection::left, x, y, z))
					addFaceToMesh(blockType, BlockDirection::left, leftFace, x, y, z);

				if(!doesFaceExist(chunk, neighbors, BlockDirection::right, x, y, z))
					addFaceToMesh(blockType, BlockDirection::right, rightFace, x, y, z);
			}
		}
	}
	indiciesSize = static_cast<GLsizei>(indices.size());
	createBuffers();

	vertices.clear();
	vertices.shrink_to_fit();
	indices.clear();
	indices.shrink_to_fit();
}

void ChunkMesh::createBuffers()
{
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Cube vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1); // Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

bool ChunkMesh::doesFaceExist(const Chunk& chunk, ChunkNeighbors neighbors, BlockDirection direction, int x, int y, int z) const
{
	glm::ivec3 adj = getAdjacentCoords(x, y, z, direction);

	if (adj.y < 0 || adj.y >= ChunkHeight)
		return false;

	if (adj.x >= 0 && adj.x < ChunkWidth && adj.z >= 0 && adj.z < ChunkDepth)
		return chunk.getBlock(adj.x, adj.y, adj.z) != BlockType::air;

	if (adj.x < 0)
		return neighbors.west  && neighbors.west->getBlock(ChunkWidth - 1, adj.y, adj.z) != BlockType::air;
	if (adj.x >= ChunkWidth)
		return neighbors.east  && neighbors.east->getBlock(0, adj.y, adj.z) != BlockType::air;
	if (adj.z < 0)
		return neighbors.south && neighbors.south->getBlock(adj.x, adj.y, ChunkDepth - 1) != BlockType::air;
	if (adj.z >= ChunkDepth)
		return neighbors.north && neighbors.north->getBlock(adj.x, adj.y, 0) != BlockType::air;

	return false;
}

void ChunkMesh::addFaceToMesh(BlockType blockType, BlockDirection dir, std::span<const float> faceVertices, int x, int y, int z)
{
	uint32_t indiciesCount = indices.empty() ? 0 : indices.back() + 4;
	TextureCoord textureCoords = Game::texture->getTextureCoords(blockType, dir);
	int textureIndex = 0; 
	for (int i = 0, index = 0; i < 4; ++i) {
		vertices.push_back(faceVertices[index++] + x);
		vertices.push_back(faceVertices[index++] + y);
		vertices.push_back(faceVertices[index++] + z);
		vertices.push_back(textureCoords[textureIndex++]);
		vertices.push_back(textureCoords[textureIndex++]);
	}
	indices.insert(indices.end(), { indiciesCount, indiciesCount + 1, indiciesCount + 2, indiciesCount + 2, indiciesCount + 3, indiciesCount });
}
