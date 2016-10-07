#pragma once
#include "../common/sharedInclude.h"
#include "../common/texture.h"
#include "../common/effect.h"
#include "../common/mesh.h"

class Frame
{
public:
	Frame(Effect &effect, Mesh &mesh, Effect &glassEffect, Mesh &glassMesh);
	Frame(const Frame &) = delete;
	Frame & operator=(const Frame &) = delete;
	void Render(glm::mat4 &view, glm::mat4 &projection, glm::vec3 lightPosition);
	glm::mat4 frameFixUp;
	glm::mat4 glassFixUp;
	glm::mat4 model;
private:
	Effect &effect;
	Mesh &mesh;

	Effect &glassEffect;
	Mesh &glassMesh;

	GLint modelUniform;
	GLint viewUniform;
	GLint projectionUniform;
	GLint lightPositionUniform;

	GLint glassModelUniform;
	GLint glassViewUniform;
	GLint glassProjectionUniform;
};
