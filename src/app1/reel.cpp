#include "reel.h"

Reel::Reel(Texture &texture, Effect &effect, Mesh &mesh, int faces) 
	: texture(texture),
	effect(effect),
	mesh(mesh),
	faces(faces)
{
	modelUniform = effect.Uniform("Model");
	viewUniform = effect.Uniform("View");
	projectionUniform = effect.Uniform("Projection");
	textureUniform = effect.Uniform("TextureUnit0");
	velocityUniform = effect.Uniform("Velocity");
	velocity = 0.0f;
	angle = 0.0f;
}

void Reel::Render(glm::mat4 &view, glm::mat4 &projection)
{
	float halfSectorAngle = glm::pi<float>() / faces;
	auto rotation = glm::rotate(glm::mat4(), angle - halfSectorAngle, glm::vec3(1, 0, 0));
	auto rotatedModel = rotation * model;
	effect.Apply();

	glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(rotatedModel));
	glUniformMatrix4fv(viewUniform, 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(projectionUniform, 1, false, glm::value_ptr(projection));
	glUniform1f(velocityUniform, velocity);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();
	glUniform1i(textureUniform, 0);

	mesh.Draw(true, true);
}

void Reel::Update(float ms)
{
	if (velocity != 0)
	{
		angle += velocity * ms;
		while (angle > 2 * glm::pi<float>())
		{
			angle -= 2 * glm::pi<float>();
		}
		while (angle < 0)
		{
			angle += 2 * glm::pi<float>();
		}

		if (velocity < 0.01f) //velocity = 0.0f;
		{

			if (RoundFace(angle) < 0.02)
			{
				velocity = 0.0f;
				float sectorAngle = 2 * glm::pi<float>() / faces;
				angle = FaceIndex(angle) * sectorAngle;
			}
			else
			{
				velocity = 0.01f;
			}
		}
		else
		{
			velocity = velocity * 0.99f;
		}
	}
}

void Reel::Roll(float velocity)
{
	this->velocity = velocity;
}

float Reel::RoundFace(float angle)
{
	float sectorAngle = 2 * glm::pi<float>() / faces;
	auto faceIndex = FaceIndex(angle);
	return glm::abs(faceIndex * sectorAngle - angle);
}

int Reel::FaceIndex(float angle)
{
	float sectorAngle = 2 * glm::pi<float>() / faces;
	auto faceIndex = glm::round(angle / sectorAngle);
	return static_cast<int>(faceIndex);
}