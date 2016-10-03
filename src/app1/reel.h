#pragma once
#include "../common/sharedInclude.h"
#include "../common/texture.h"
#include "../common/effect.h"
#include "../common/mesh.h"

class Reel
{
public:
	Reel(Texture &texture, Effect &effect, Mesh &mesh);
	Reel(const Reel &) = delete;
	Reel & operator=(const Reel &) = delete;
	void Render(glm::mat4 &view, glm::mat4 &projection);
	glm::mat4 model;
private:
	Texture &texture;
	Effect &effect;
	Mesh &mesh;

	GLint modelUniform;
	GLint viewUniform;
	GLint projectionUniform;
	GLint textureUniform;
};

void CreateReelMesh(Mesh &mesh, int faces);
