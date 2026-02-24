#include "ChunkMesh.h"
#include "Game.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/matrix.hpp"

#include "glad/glad.h"
//#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

// Face drawing order
// Bottom left, top left, top right, bottom right

std::vector<GLfloat> frontFace = {
	0, 0, 1,
	0, 1, 1,
	1, 1, 1,
	1, 0, 1
};

std::vector<GLfloat> backFace = {
	0, 0, 0,
	0, 1, 0,
	1, 1, 0,
	1, 0, 0
};

std::vector<GLfloat> topFace = {
	0, 1, 1,
	0, 1, 0,
	1, 1, 0,
	1, 1, 1
};

std::vector<GLfloat> bottomFace = {
	0, 0, 1,
	0, 0, 0,
	1, 0, 0,
	1, 0, 1
};

std::vector<GLfloat> leftFace = {
	0, 0, 0,
	0, 1, 0,
	0, 1, 1,
	0, 0, 1
};

std::vector<GLfloat> rightFace = {
	1, 0, 1,
	1, 1, 1,
	1, 1, 0,
	1, 0, 0
};

std::vector<GLuint> ebo = {
	0, 1, 2,
	2, 3, 0
};

//std::vector<GLfloat> textureCoords = {
//	0.0f, 0.0f, // top left
//	0.0f, 1.0f, // bottom left
//	0.333f, 1.0f, // bottom right
//	0.333f, 0.0f // top left
//};

ChunkMesh::ChunkMesh(Chunk* chunk)
    : shader("shaders/shader.vert", "shaders/shader.frag") {
	// Why are the shaders names hardcoded?
	// Is it possible to have these in bytecode? Do I need plain-text stuff in the .exe dir?
	this->chunk = chunk;
}

ChunkMesh::~ChunkMesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void ChunkMesh::draw(glm::vec3& position, Camera& camera) {
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
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ChunkMesh::createMesh()
{
	/*
	for(int x = 0; x < 16; x++) {
		for(int y = 0; y < 16; y++) {
			for(int z = 0; z < 16; z++) {
				BlockType type = blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE];
				if(type == BlockType::air)
					continue;
				// Front
				if(!doesFaceExist(BlockDirection::front, x, y, z))
					addFaceToMesh(type, BlockDirection::front, frontFace, x, y, z);
				// Back
				if(!doesFaceExist(BlockDirection::back, x, y, z))
					addFaceToMesh(type, BlockDirection::back, backFace, x, y, z);
				// Top
				if(!doesFaceExist(BlockDirection::top, x, y, z))
					addFaceToMesh(type, BlockDirection::top, topFace, x, y, z);
				// Bottom
				if(!doesFaceExist(BlockDirection::bottom, x, y, z))
					addFaceToMesh(type, BlockDirection::bottom, bottomFace, x, y, z);
				// Left
				if(!doesFaceExist(BlockDirection::left, x, y, z))
					addFaceToMesh(type, BlockDirection::left, leftFace, x, y, z);
				// Right
				if(!doesFaceExist(BlockDirection::right, x, y, z))
					addFaceToMesh(type, BlockDirection::right, rightFace, x, y, z);
			}
		}
	}
	*/
	for (int x = 0; x < LayerSize; ++x) {
		for (int y = 0; y < LayerSize; ++y) {
			for (int z = 0; z < LayerSize; ++z) {
				BlockType bt = this->chunk->getBlock(x, y, z);
				if(bt == BlockType::air)
					continue;
				if(!doesFaceExist(BlockDirection::front, x, y, z))
					addFaceToMesh(bt, BlockDirection::front, frontFace, x, y, z);

				if(!doesFaceExist(BlockDirection::back, x, y, z))
					addFaceToMesh(bt, BlockDirection::back, backFace, x, y, z);

				if(!doesFaceExist(BlockDirection::top, x, y, z))
					addFaceToMesh(bt, BlockDirection::top, topFace, x, y, z);

				if(!doesFaceExist(BlockDirection::bottom, x, y, z))
					addFaceToMesh(bt, BlockDirection::bottom, bottomFace, x, y, z);

				if(!doesFaceExist(BlockDirection::left, x, y, z))
					addFaceToMesh(bt, BlockDirection::left, leftFace, x, y, z);

				if(!doesFaceExist(BlockDirection::right, x, y, z))
					addFaceToMesh(bt, BlockDirection::right, rightFace, x, y, z);
			}
		}
	}
	createBuffers();
}

void ChunkMesh::createBuffers()
{
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

bool ChunkMesh::doesFaceExist(BlockDirection direction, int x, int y, int z) const
{
	int adjX = x;
	int adjY = y;
	int adjZ = z;
	// think about a jump table if the value is "clamped"
	if(direction == BlockDirection::top) {
		adjY = y + 1;
	} else if(direction == BlockDirection::bottom) {
		adjY = y - 1;
	} else if(direction == BlockDirection::front) {
		adjZ = z + 1;
	} else if(direction == BlockDirection::back) {
		adjZ = z - 1;
	} else if(direction == BlockDirection::left) {
		adjX = x - 1;
	} else if(direction == BlockDirection::right) {
		adjX = x + 1;
	}

	// Guard against array out-of-bounds
	// Chunks can only see their own block data for now.
	if(adjX < 0 || adjY < 0 || adjZ < 0 || adjX > 15 || adjY > 15 || adjZ > 15)
		return false;

	if(this->chunk->getBlock(adjX, adjY, adjZ) != BlockType::air)
		return true;

	return false;
}

void ChunkMesh::addFaceToMesh(BlockType blockType, BlockDirection dir, const std::vector<GLfloat>& faceVertices, int x, int y, int z)
{
	unsigned int indicesCount;
	if(indices.empty())
		indicesCount = 0;
	else
		indicesCount = indices.back() + 4;

	std::vector<GLfloat> textureCoords = Game::texture->getTextureCoords(blockType, dir);
	
	int textureIndex = 0;
	for (int i = 0, index = 0; i < 4; ++i) {
		vertices.push_back(faceVertices[index++] + x);
		vertices.push_back(faceVertices[index++] + y);
		vertices.push_back(faceVertices[index++] + z);
		vertices.push_back(textureCoords[textureIndex++]);
		vertices.push_back(textureCoords[textureIndex++]);
	}
	indices.insert(indices.end(), { indicesCount, indicesCount + 1, indicesCount + 2, indicesCount + 2, indicesCount + 3, indicesCount });
}
