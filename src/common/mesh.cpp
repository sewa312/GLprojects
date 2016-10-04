#include <memory>
#include "mesh.h"



void Mesh::Draw(bool useNormals, bool useTexture)
{
	UseVertexAttribPointer position(VertexAttr::POSITION, 3, GL_FLOAT, false, 0, &Vertices[0]);
	auto normal = useNormals ? std::make_unique<UseVertexAttribPointer>(
		VertexAttr::NORMAL, 3, GL_FLOAT, false, 0, &Normals[0]) : nullptr;
	auto texture = useTexture ? std::make_unique<UseVertexAttribPointer>(
		VertexAttr::TEXCOORD0, 2, GL_FLOAT, false, 0, &Coords[0]) : nullptr;
	glDrawElements(GL_TRIANGLES, 3 * Triangles.size(), GL_UNSIGNED_INT, &Triangles[0]);
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