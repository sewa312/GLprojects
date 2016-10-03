#pragma once

//#define GLEW_STATIC
#pragma comment(lib, "glew32.lib")
#include <GL/glew.h>

#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/constants.hpp>

namespace VertexAttr 
{
	enum VertexAttr 
	{
		POSITION = 0,
		COLOR = 1,
		NORMAL = 2,
		TEXCOORD0 = 3,
		TANGENT = 4,
		BITANGENT = 5,
	};
}

struct UseVertexAttribPointer
{
	UseVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
		: attributeIndex(index)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	~UseVertexAttribPointer()
	{
		glDisableVertexAttribArray(attributeIndex);
	}

	operator GLuint() { return attributeIndex; }

	GLuint attributeIndex;
};

struct UseCapability
{
	UseCapability(GLenum cap, bool enable)
		: cap(cap)
	{
		previousState = glIsEnabled(cap);
		if (enable)
			glEnable(cap);
		else
			glDisable(cap);
	}

	~UseCapability()
	{
		if (previousState)
			glEnable(cap);
		else
			glDisable(cap);
	}

	GLenum cap;
	GLboolean previousState;
};