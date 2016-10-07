#include <memory>
#include <iostream>
#include "mesh.h"
#include "utils.h"



void Mesh::Draw(bool useNormals, bool useTexture)
{
	UseVertexAttribPointer position(VertexAttr::POSITION, 3, GL_FLOAT, false, 0, &Vertices[0]);
	auto normal = useNormals ? std::make_unique<UseVertexAttribPointer>(
		VertexAttr::NORMAL, 3, GL_FLOAT, false, 0, &Normals[0]) : nullptr;
	auto texture = useTexture ? std::make_unique<UseVertexAttribPointer>(
		VertexAttr::TEXCOORD0, 2, GL_FLOAT, false, 0, &Coords[0]) : nullptr;
	glDrawElements(GL_TRIANGLES, 3 * Triangles.size(), GL_UNSIGNED_INT, &Triangles[0]);
}

void CreateFrameMesh(Mesh &mesh)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec3 > temp_normals;

	FILE * file = fopen("objects/frame.obj", "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");    return ;
	}

	int i = 0;

	while (1) {
		char lineHeader[1024];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
				&vertexIndex[1], &normalIndex[1],
				&vertexIndex[2], &normalIndex[2]);
			if (matches != 6) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return;
			}

			mesh.Vertices.push_back(temp_vertices.at(vertexIndex[0] - 1));
			mesh.Vertices.push_back(temp_vertices.at(vertexIndex[1] - 1));
			mesh.Vertices.push_back(temp_vertices.at(vertexIndex[2] - 1));

			mesh.Normals.push_back(temp_normals.at(normalIndex[0] - 1));
			mesh.Normals.push_back(temp_normals.at(normalIndex[1] - 1));
			mesh.Normals.push_back(temp_normals.at(normalIndex[2] - 1));

			mesh.Triangles.push_back(glm::uvec3(i + 1, i, i + 2));
			i = i + 3;
		}
	}
}

std::tuple<glm::vec3, glm::vec3> CalculateBBox(const Mesh &mesh)
{
	if (mesh.Vertices.size() == 0)
	{
		return std::make_tuple(glm::vec3(), glm::vec3());
	}
	else
	{
		auto min = mesh.Vertices[0];
		auto max = mesh.Vertices[0];

		for (size_t i = 1; i < mesh.Vertices.size(); i++)
		{
			min = glm::min(min, mesh.Vertices[i]);
			max = glm::max(max, mesh.Vertices[i]);
		}

		return std::make_tuple(min, max);
	}
}

namespace
{
	int const cubeData[][7] = {
		{ 0, 4, 2, 6, -1, 0, 0 }, // -x
		{ 1, 3, 5, 7, +1, 0, 0 }, // +x
		{ 0, 1, 4, 5, 0, -1, 0 }, // -y
		{ 2, 6, 3, 7, 0, +1, 0 }, // +y
		{ 0, 2, 1, 3, 0, 0, -1 }, // -z
		{ 4, 5, 6, 7, 0, 0, +1 }, // +z
	};

	glm::ivec3 pickOctant(int i)
	{
		return glm::ivec3((i & 1) * 2 - 1, (i & 2) - 1, (i & 4) / 2 - 1);
	}
};

void CreateCube(Mesh &mesh)
{
	for (int i = 0; i < 6; i++) {
		int v = i * 4;
		for (int j = 0; j < 4; j++) {
			int d = cubeData[i][j];
			glm::ivec3 oct = pickOctant(d);
			mesh.Vertices.push_back(glm::vec3(oct.x, oct.y, oct.z));
			mesh.Coords.push_back(glm::vec2(j & 1, (j & 2) / 2));
			mesh.Normals.push_back(glm::vec3(cubeData[i][4], cubeData[i][5], cubeData[i][6]));
		}
		mesh.Triangles.push_back(glm::uvec3(v + 1, v, v + 2));
		mesh.Triangles.push_back(glm::uvec3(v + 1, v + 2, v + 3));
	}
}