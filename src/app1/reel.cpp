#include "reel.h"

Reel::Reel(Texture &texture, Effect &effect, Mesh &mesh) 
	: texture(texture),
	effect(effect),
	mesh(mesh)
{
	modelUniform = effect.Uniform("Model");
	viewUniform = effect.Uniform("View");
	projectionUniform = effect.Uniform("Projection");
	textureUniform = effect.Uniform("TextureUnit0");
}

void Reel::Render(glm::mat4 &view, glm::mat4 &projection)
{
	effect.Apply();

	glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(model));
	glUniformMatrix4fv(viewUniform, 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(projectionUniform, 1, false, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();
	glUniform1i(textureUniform, 0);

	mesh.Draw(true, true);
}