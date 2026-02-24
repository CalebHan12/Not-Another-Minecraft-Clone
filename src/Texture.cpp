#include "Texture.h"
#include "ChunkMesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

/*
TODO: ...
Have texture coords be read in from file
File will contain coordinate system, grass block top 0,0 and so on, multiply by texture atlas width and length
Make function that retrieves these functions, store in std::unorderedmap
Do I have to make block enum? Probably better to bite the bullet
*/

std::vector<int> grassFrontCoords = { 0, 0 };
std::vector<int> grassTopCoords = { 0, 1 };
std::vector<int> grassBottomCoords = { 0, 2 };

Texture::Texture()
{ 
}

Texture::Texture(const std::filesystem::path& path)
{
	if(path.empty() || !std::filesystem::exists(path))
		return;

	this->texturePath = path;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	int nrChannels;
	unsigned char* data = stbi_load(path.string().c_str(), &this->width, &this->height, &nrChannels, 0);
	if(data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture at " << path << std::endl;
	}
	stbi_image_free(data);
}

Texture::~Texture()
{
}

void Texture::useTexture() const
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::loadAtlas(const std::filesystem::path& path)
{
	if(path.empty() || !std::filesystem::exists(path))
		return;

	this->texturePath = path;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	int nrChannels;
	unsigned char* data = stbi_load(path.string().c_str(), &this->width, &this->height, &nrChannels, 0);
	if(!data)
		std::cout << "Failed to load texture at " << path << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

std::vector<GLfloat> Texture::getTextureCoords(BlockType type, BlockDirection dir)
{
	// Each texture is 16x16 
	// Bottom left, top left, top right, bottom right

	std::vector<GLfloat> texCoords(8);
	if(type == BlockType::grass) {
		if(dir == BlockDirection::left || dir == BlockDirection::front 
			|| dir == BlockDirection::back || dir == BlockDirection::right)
			return { 0.0f, 0.8f, 0.0f, 0.9875f, 0.1875f, 0.9875f, 0.1875f, 0.8f };
		else if(dir == BlockDirection::top)
			return { 0.2f, 0.8f, 0.2f, 0.9875f, 0.3875f, 0.9875f, 0.3875f, 0.8f };
		else if(dir == BlockDirection::bottom)
			return { 0.4f, 0.8f, 0.4f, 0.9875f, 0.5875f, 0.9875f, 0.5875f, 0.8f };
	} else if(type == BlockType::dirt) {
		return { 0.4f, 0.8f, 0.4f, 0.9875f, 0.5875f, 0.9875f, 0.5875f, 0.8f };
	}
	return { 0.0f, 0.8f, 0.0f, 0.9875f, 0.1875f, 0.9875f, 0.1875f, 0.8f };
	// (2i + 1)/(2N)
}
