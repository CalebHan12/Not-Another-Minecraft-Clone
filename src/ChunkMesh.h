#pragma once
#include <vector>

#include "Blocks.h"
#include "Camera.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glad/glad.h"

class Chunk;

class ChunkMesh {
public:
	ChunkMesh() = default;
	~ChunkMesh();

	ChunkMesh(const ChunkMesh& other) = default;
	ChunkMesh(ChunkMesh&& other) noexcept = default;

	ChunkMesh& operator=(const ChunkMesh& other) = default;
	ChunkMesh& operator=(ChunkMesh&& other) noexcept = default;

	void draw(glm::vec3& position, Camera& camera, Shader& shader);
	void createMesh(const Chunk& chunk);

private:
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	void createBuffers();
	[[nodiscard]] bool doesFaceExist(const Chunk& chunk, BlockDirection direction, int x, int y, int z) const;
	void addFaceToMesh(BlockType blockType, BlockDirection dir, const std::vector<GLfloat>& faceVertices,
						int x, int y, int z);
};
