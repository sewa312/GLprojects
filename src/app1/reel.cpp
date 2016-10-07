#include "reel.h"

Reel::Reel(Texture &texture, Effect &effect, Mesh &mesh, int faces) 
	: texture(texture),
	effect(effect),
	mesh(mesh),
	faces(faces),
	roll(false),
	stop(true)
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

		if (velocity < 0.005f && !roll) //velocity = 0.0f;
		{

			if (RoundFace(angle) < 0.02)
			{
				velocity = 0.0f;
				stop = true;
				float sectorAngle = 2 * glm::pi<float>() / faces;
				angle = FaceIndex(angle) * sectorAngle;
			}
			else
			{
				velocity = 0.004f;
			}
		}
		else
		{
			velocity = velocity * 0.98f;
		}
	}
}

void Reel::Roll(float velocity)
{
	this->velocity = velocity;
	roll = true;
	stop = false;
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

void Reel::Stop()
{
	roll = false;
}

bool Reel::IsStop()
{
	return stop;
}

void CreateReelMesh(Mesh &mesh, int faces)
{
	mesh.Vertices.push_back(glm::vec3(-1, 0, 0));
	mesh.Vertices.push_back(glm::vec3(+1, 0, 0));

	mesh.Normals.push_back(glm::vec3(-1, 0, 0));
	mesh.Normals.push_back(glm::vec3(+1, 0, 0));

	mesh.Coords.push_back(glm::vec2(0, 0));
	mesh.Coords.push_back(glm::vec2(1, 0));

	for (int i = 0; i < faces + 1; i++)
	{
		float angle = 2 * glm::pi<float>() * i / faces;
		float y = glm::sin(angle);
		float z = glm::cos(angle);
		mesh.Vertices.push_back(glm::vec3(-1, y, z));
		mesh.Vertices.push_back(glm::vec3(+1, y, z));

		mesh.Normals.push_back(glm::vec3(0, y, z));
		mesh.Normals.push_back(glm::vec3(0, y, z));

		mesh.Coords.push_back(glm::vec2(0.10f, i * 1.0f / faces));
		mesh.Coords.push_back(glm::vec2(0.95f, i * 1.0f / faces));

		if (i < faces)
		{
			int j1 = i + 1;
			int j2 = i + 2;
			mesh.Triangles.push_back(glm::uvec3(0, 2 * j2, 2 * j1));
			mesh.Triangles.push_back(glm::uvec3(1, 2 * j1 + 1, 2 * j2 + 1));
			mesh.Triangles.push_back(glm::uvec3(2 * j1, 2 * j2, 2 * j2 + 1));
			mesh.Triangles.push_back(glm::uvec3(2 * j2 + 1, 2 * j1 + 1, 2 * j1));
		}
	}
}