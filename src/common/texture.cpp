#include "texture.h"

#include <fstream>
#include <vector>
#include <string>
#include <assert.h>
#include "utils.h"


Texture::Texture()
	: id(0), width(0), height(0), components(0), format(0)
{
}

Texture::~Texture()
{
	if (glIsTexture(id))
		glDeleteTextures(1, &id);
}

void Texture::Bind()
{
	Bind(0);
}

void Texture::Bind(GLuint index)
{
	assert(id != 0);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::AllocateTextureObject()
{
	assert(id == 0);
	glGenTextures(1, &id);
	if (id <= 0)
		throw std::runtime_error("Failed to allocate texture object.");
	Bind();
}

void Texture::LoadFromFile(const char *fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	auto pixelData = ReadTGABits(fileName, width, height, components, format);

	AllocateTextureObject();

	// set wrapping and filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLfloat largestAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largestAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largestAnisotropy);

	// load pixel data into server memory
	glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, &pixelData[0]);

	if (minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR || minFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void Texture::InitializeEmpty(GLint width, GLint height, GLenum minFilter, GLenum magFilter, GLenum type)
{
	AllocateTextureObject();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	this->width = width;
	this->height = height;
	this->components = type == GL_FLOAT ? GL_RGBA32F : GL_RGBA;

	std::vector<char> data;
	data.resize(width * height * 4 * (type == GL_FLOAT ? 4 : 1));
	glTexImage2D(GL_TEXTURE_2D, 0, this->components, width, height, 0, GL_RGBA, type, &data[0]);
}
