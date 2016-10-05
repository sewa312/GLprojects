#pragma once
#include "../common/sharedInclude.h"
#include "../common/texture.h"
#include "../common/effect.h"
#include "../common/mesh.h"

class Reel
{
public:
	Reel(Texture &texture, Effect &effect, Mesh &mesh, int faces);
	Reel(const Reel &) = delete;
	Reel & operator=(const Reel &) = delete;
	void Render(glm::mat4 &view, glm::mat4 &projection);
	void Update(float ms);
	void Roll(float velocity);
	float RoundFace(float angle);
	int FaceIndex(float angle);
	glm::mat4 model;
private:
	Texture &texture;
	Effect &effect;
	Mesh &mesh;

	GLint modelUniform;
	GLint viewUniform;
	GLint projectionUniform;
	GLint textureUniform;

	float velocity;
	float angle;
	int faces;
};

void CreateReelMesh(Mesh &mesh, int faces);
