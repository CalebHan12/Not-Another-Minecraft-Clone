#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

constexpr int tilePixelSize = 16;
constexpr float texEpsilon = 0.0005f;

// Maps [BlockType][BlockDirection] -> atlas tile index (left-to-right, top-to-bottom).
// BlockDirection order: top=0, bottom=1, front=2, back=3, left=4, right=5
static const int blockTileMap[4][6] = {
	{ 0, 0, 0, 0, 0, 0 },  // air (unused)
	{ 1, 2, 0, 0, 0, 0 },  // grass: top=1, bottom=2(dirt), sides=0
	{ 2, 2, 2, 2, 2, 2 },  // dirt: all=2
	{ 4, 4, 4, 4, 4, 4 },  // stone: all=3
};

Texture::Texture(const std::filesystem::path& path)
{
	loadAtlas(path);
}

Texture::~Texture() {}

void Texture::useTexture() const
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::loadAtlas(const std::filesystem::path& path)
{
	if(path.empty() || !std::filesystem::exists(path))
		return;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
	if(!data) {
		std::cout << "Failed to load texture at " << path << std::endl;
		return;
	}

	GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	loadTextureCoords(width, height);
}

void Texture::loadTextureCoords(int atlasWidth, int atlasHeight)
{
	int tilesPerRow = atlasWidth / tilePixelSize;
	int tilesPerCol = atlasHeight / tilePixelSize;
	int totalTiles = tilesPerRow * tilesPerCol;

	float uTile = (float)tilePixelSize / atlasWidth;
	float vTile = (float)tilePixelSize / atlasHeight;

	uvs.resize(totalTiles);

	for (int i = 0; i < totalTiles; i++) {
		int col = i % tilesPerRow;
		int row = i / tilesPerRow;

		// stbi vertical flip maps atlas row 0 (top of image) to the highest v values.
		int flippedRow = tilesPerCol - 1 - row;

		float uMin = col * uTile + texEpsilon;
		float vMin = flippedRow * vTile + texEpsilon;
		float uMax = uMin + uTile - 2 * texEpsilon;
		float vMax = vMin + vTile - 2 * texEpsilon;

		uvs[i] = { uMin, uMax, vMin, vMax };
	}
}

TextureCoord Texture::getTextureCoords(BlockType type, BlockDirection dir) const
{
	int tileIdx = blockTileMap[static_cast<int>(type)][static_cast<int>(dir)];

	if(tileIdx < 0 || tileIdx >= static_cast<int>(uvs.size()))
		return { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };

	const UV& uv = uvs[tileIdx];
	// Bottom-left, top-left, top-right, bottom-right (matches ChunkMesh vertex order)
	return {
		uv.uMin, uv.vMin,
		uv.uMin, uv.vMax,
		uv.uMax, uv.vMax,
		uv.uMax, uv.vMin
	};
}
