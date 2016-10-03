#pragma once
#include <vector>

#include "sharedInclude.h"

class Mesh
{
public:
	std::vector<glm::vec3> Vertices;
	std::vector<glm::uvec3> Triangles;
	std::vector<glm::vec2> Coords;
	std::vector<glm::vec3> Normals;
	void Draw(bool useNormals, bool useTexture);

private:

};
