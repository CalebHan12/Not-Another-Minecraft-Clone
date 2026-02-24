#pragma once
#include <vector>

#include "Camera.h"
#include "Chunk.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glad/glad.h"

class ChunkMesh
{
public:
	ChunkMesh(Chunk* chunk);
	~ChunkMesh();
	void draw(glm::vec3& position, Camera& camera);
	void createMesh();

private:
	Shader shader;
	
	GLuint vao = 0, vbo = 0, ebo = 0;

	// non-owning pointer - hell yea
	Chunk* chunk;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	void createBuffers();
	[[nodiscard]] bool doesFaceExist(BlockDirection direction, int x, int y, int z) const;
	void addFaceToMesh(BlockType blockType, BlockDirection dir, const std::vector<GLfloat>& faceVertices,
						int x, int y, int z);
};
