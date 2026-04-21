#pragma once
#include <array>
#include <span>
#include <vector>

#include "Blocks.h"
#include "Camera.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glad/glad.h"

class Chunk;

struct ChunkNeighbors {
    const Chunk* north = nullptr; // +z
    const Chunk* south = nullptr; // -z
    const Chunk* east  = nullptr; // +x
    const Chunk* west  = nullptr; // -x
};

class ChunkMesh {
public:
	ChunkMesh() = default;
	~ChunkMesh();

	ChunkMesh(const ChunkMesh& other) = delete;
	ChunkMesh(ChunkMesh&& other) noexcept = default;

	ChunkMesh& operator=(const ChunkMesh& other) = delete;
	ChunkMesh& operator=(ChunkMesh&& other) noexcept = default;

	void draw(const glm::vec3& position, Camera& camera, Shader& shader) const;
	void createMesh(const Chunk& chunk, ChunkNeighbors neighbors = {});

private:
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLsizei indiciesSize = 0;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	void createBuffers();
	[[nodiscard]] bool doesFaceExist(const Chunk& chunk, ChunkNeighbors neighbors, BlockDirection direction, int x, int y, int z) const;
	void addFaceToMesh(BlockType blockType, BlockDirection dir, std::span<const float> faceVertices,
						int x, int y, int z);
};
