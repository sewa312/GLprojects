#pragma once

#include "sharedinclude.h"

class Texture
{
public:
	Texture();
	~Texture();

	GLuint ID() const { return id; }
	void Bind();
	void Bind(GLuint index);

	GLint Width() const { return width; }
	GLint Height() const { return height; }
	GLenum Format() const { return format; }

	void LoadFromFile(const char *fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void InitializeEmpty(GLint width, GLint height, GLenum minFilter, GLenum magFilter, GLenum type);

private:
	Texture(const Texture &) {}
	Texture & operator=(const Texture &) {}

	void AllocateTextureObject();

	GLuint id;
	GLint width, height, components;
	GLenum format;

};
