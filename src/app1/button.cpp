#include <vector>

#include "button.h"

namespace
{
	std::vector<glm::vec2> Coords =
	{
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },


	};

	std::vector<glm::uvec3> Triangles = 
	{
		{ 1, 0, 2 },
		{ 2, 3, 1 },
	};
}

Button::Button(Texture &texture, Effect &effect)
	: texture(texture),
	effect(effect)
{
	viewportUniform = effect.Uniform("Viewport");
	textureUniform = effect.Uniform("TextureUnit0");
	timeUniform = effect.Uniform("Time");
}

void Button::Render(glm::vec2 viewport, float absoluteTimeMs)
{
	std::vector<glm::vec2> Vertices =
	{
		{ position.x + size.x, position.y + size.y },
		{ position.x + size.x, position.y },
		{ position.x, position.y + size.y },
		{ position.x, position.y },
	};

	UseCapability depth(GL_DEPTH_TEST, false);
	UseCapability culling(GL_CULL_FACE, false);

	effect.Apply();

	glUniform2fv(viewportUniform, 1, glm::value_ptr(viewport));
	glUniform1f(timeUniform, absoluteTimeMs);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();
	glUniform1i(textureUniform, 0);

	UseVertexAttribPointer position(VertexAttr::POSITION, 2, GL_FLOAT, false, 0, &Vertices[0]);
	UseVertexAttribPointer texture(VertexAttr::TEXCOORD0, 2, GL_FLOAT, false, 0, &Coords[0]);
	glDrawElements(GL_TRIANGLES, 3 * Triangles.size(), GL_UNSIGNED_INT, &Triangles[0]);
}

bool Button::ContainsPoint(glm::vec2 point)
{
	return point.x >= position.x && point.x < (position.x +size.x) && 
		point.y >= position.y && point.y < (position.y + size.y);
}
