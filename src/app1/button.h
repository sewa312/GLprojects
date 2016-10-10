#pragma once
#include "../common/sharedInclude.h"
#include "../common/texture.h"
#include "../common/effect.h"
#include "../common/mesh.h"

class Button
{
public:
	Button(Texture &texture, Effect &effect);
	Button(const Button &) = delete;
	Button & operator=(const Button &) = delete;
	void Render(glm::vec2 viewport, float absoluteTimeMs);
	bool ContainsPoint(glm::vec2 point);
	glm::vec2 position;
	glm::vec2 size;
private:
	Texture &texture;
	Effect &effect;

	GLint viewportUniform;
	GLint textureUniform;
	GLint timeUniform;
};
