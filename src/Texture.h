#pragma once

#include <array>
#include <filesystem>
#include <vector>

#include "Blocks.h"
#include "glad/glad.h"

using TextureCoord = std::array<GLfloat, 8>;

struct UV {
	float uMin, uMax;
	float vMin, vMax;
};

class Texture
{
public:
	Texture(const std::filesystem::path& path);
	~Texture();
	void useTexture() const;
	[[nodiscard]] TextureCoord getTextureCoords(BlockType type, BlockDirection dir) const;

private:
	GLuint texture = 0;
	std::vector<UV> uvs;
	void loadAtlas(const std::filesystem::path& path);
	void loadTextureCoords(int atlasWidth, int atlasHeight);
};
