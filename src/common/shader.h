
#pragma once

#include <string>
#include "sharedinclude.h"
#include "shaderLoadException.h"

class Shader
{
public:
	explicit Shader(GLenum type);
	Shader(Shader &&);
	Shader & operator=(Shader &&);
	~Shader();

	GLuint ID() const { return id; }

	Shader & CompileText(const char *source);
	Shader & CompileFile(const char *fileName);

private:
	Shader(const Shader &) {}
	Shader & operator=(const Shader &) {}

	void Validate();

	GLenum type;
	GLuint id;
};
