#include "frame.h"

Frame::Frame(Effect &effect, Mesh &mesh, Effect &glassEffect, Mesh &glassMesh)
	: effect(effect),
	mesh(mesh),
	glassEffect(glassEffect),
	glassMesh(glassMesh)
{
	modelUniform = effect.Uniform("Model");
	viewUniform = effect.Uniform("View");
	projectionUniform = effect.Uniform("Projection");
	lightPositionUniform = effect.Uniform("LightPosition_worldspace");

	glassModelUniform = glassEffect.Uniform("Model");
	glassViewUniform = glassEffect.Uniform("View");
	glassProjectionUniform = glassEffect.Uniform("Projection");

	auto frameModel = glm::mat4();
	frameModel = glm::rotate(glm::mat4(), glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0)) * frameModel;
	frameModel = glm::rotate(glm::mat4(), glm::pi<float>() / 2.0f, glm::vec3(0, 0, 1)) * frameModel;
	frameModel = glm::translate(glm::mat4(), glm::vec3(0.85f, -0.67f, -1.0f)) * frameModel;
	frameModel = glm::scale(glm::mat4(), glm::vec3(2.0f, 1.65f, 1.0f)) * frameModel;
	frameFixUp = frameModel;

	auto glassModel = glm::mat4();
	glassModel = glm::scale(glm::mat4(), glm::vec3(2.2f, 1.15f, 0.04f)) * glassModel;
	glassFixUp = glassModel;
}

void Frame::Render(glm::mat4 &view, glm::mat4 &projection, glm::vec3 lightPosition)
{
	effect.Apply();

	auto frameModel = model * frameFixUp;

	glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(frameModel));
	glUniformMatrix4fv(viewUniform, 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(projectionUniform, 1, false, glm::value_ptr(projection));
	glUniform3fv(lightPositionUniform, 1, glm::value_ptr(lightPosition));

	mesh.Draw(true, false);
	
	glassEffect.Apply();

	auto glassModel = model * glassFixUp;

	glUniformMatrix4fv(glassModelUniform, 1, false, glm::value_ptr(glassModel));
	glUniformMatrix4fv(glassViewUniform, 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glassProjectionUniform, 1, false, glm::value_ptr(projection));

	{
		UseCapability c1(GL_BLEND, true);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glassMesh.Draw(false, true);
	}
}
