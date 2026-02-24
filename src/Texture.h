#pragma once

#include <filesystem>
#include <vector>

#include "Chunk.h"
#include "glad/glad.h"

class Texture
{
public:
	Texture();
	Texture(const std::filesystem::path& path);
	~Texture();
	void useTexture() const;
	void loadAtlas(const std::filesystem::path& path);
	std::vector<GLfloat> getTextureCoords(BlockType type, BlockDirection dir);
private:
	std::filesystem::path texturePath;
	GLuint texture;
	int height, width;
};

