#ifndef MESHCREATOR_H_INCLUDED
#define MESHCREATOR_H_INCLUDED

#include <glm\glm.hpp>
#include "mesh.h"

namespace MeshCreator
{
	Mesh* GetCubeMesh(float width, float height, float depth, int texCoordScale = 1)
	{
		Vertex vertices[] =
		{
			Vertex(glm::vec3(-width, -height, -depth), glm::vec2(1 * texCoordScale, 0 * texCoordScale), glm::vec3(0, 0, -1)),
			Vertex(glm::vec3(-width, height, -depth), glm::vec2(0 * texCoordScale, 0 * texCoordScale), glm::vec3(0, 0, -1)),
			Vertex(glm::vec3(width, height, -depth), glm::vec2(0 * texCoordScale, 1 * texCoordScale), glm::vec3(0, 0, -1)),
			Vertex(glm::vec3(width, -height, -depth), glm::vec2(1 * texCoordScale, 1 * texCoordScale), glm::vec3(0, 0, -1)),

			Vertex(glm::vec3(-width, -height, depth), glm::vec2(1 * texCoordScale, 0 * texCoordScale), glm::vec3(0, 0, 1)),
			Vertex(glm::vec3(-width, height, depth), glm::vec2(0 * texCoordScale, 0 * texCoordScale), glm::vec3(0, 0, 1)),
			Vertex(glm::vec3(width, height, depth), glm::vec2(0 * texCoordScale, 1 * texCoordScale), glm::vec3(0, 0, 1)),
			Vertex(glm::vec3(width, -height, depth), glm::vec2(1 * texCoordScale, 1 * texCoordScale), glm::vec3(0, 0, 1)),

			Vertex(glm::vec3(-width, -height, -depth), glm::vec2(0 * texCoordScale, 1 * texCoordScale), glm::vec3(0, -1, 0)),
			Vertex(glm::vec3(-width, -height, depth), glm::vec2(1 * texCoordScale, 1 * texCoordScale), glm::vec3(0, -1, 0)),
			Vertex(glm::vec3(width, -height, depth), glm::vec2(1 * texCoordScale, 0 * texCoordScale), glm::vec3(0, -1, 0)),
			Vertex(glm::vec3(width, -height, -depth), glm::vec2(0 * texCoordScale, 0 * texCoordScale), glm::vec3(0, -1, 0)),

			Vertex(glm::vec3(-width, height, -depth), glm::vec2(0 * texCoordScale, 1 * texCoordScale), glm::vec3(0, 1, 0)),
			Vertex(glm::vec3(-width, height, depth), glm::vec2(1 * texCoordScale, 1 * texCoordScale), glm::vec3(0, 1, 0)),
			Vertex(glm::vec3(width, height, depth), glm::vec2(1 * texCoordScale, 0 * texCoordScale), glm::vec3(0, 1, 0)),
			Vertex(glm::vec3(width, height, -depth), glm::vec2(0 * texCoordScale, 0 * texCoordScale), glm::vec3(0, 1, 0)),

			Vertex(glm::vec3(-width, -height, -depth), glm::vec2(1 * texCoordScale, 1 * texCoordScale), glm::vec3(-1, 0, 0)),
			Vertex(glm::vec3(-width, -height, depth), glm::vec2(1 * texCoordScale, 0 * texCoordScale), glm::vec3(-1, 0, 0)),
			Vertex(glm::vec3(-width, height, depth), glm::vec2(0 * texCoordScale, 0 * texCoordScale), glm::vec3(-1, 0, 0)),
			Vertex(glm::vec3(-width, height, -depth), glm::vec2(0 * texCoordScale, 1 * texCoordScale), glm::vec3(-1, 0, 0)),

			Vertex(glm::vec3(width, -height, -depth), glm::vec2(1 * texCoordScale, 1 * texCoordScale), glm::vec3(1, 0, 0)),
			Vertex(glm::vec3(width, -height, depth), glm::vec2(1 * texCoordScale, 0 * texCoordScale), glm::vec3(1, 0, 0)),
			Vertex(glm::vec3(width, height, depth), glm::vec2(0 * texCoordScale, 0 * texCoordScale), glm::vec3(1, 0, 0)),
			Vertex(glm::vec3(width, height, -depth), glm::vec2(0 * texCoordScale, 1 * texCoordScale), glm::vec3(1, 0, 0)),
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			0, 2, 3,

			6, 5, 4,
			7, 6, 4,

			10, 9, 8,
			11, 10, 8,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			22, 21, 20,
			23, 22, 20
		};

		Mesh* mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
		return mesh;
	}

	Mesh* GetPlaneMesh(float width, float depth, int texCoordScale = 1)
	{
		Vertex vertices[] =
		{
			Vertex(glm::vec3(-width, 0, -depth), glm::vec2(0 * texCoordScale, 1 * texCoordScale), glm::vec3(0, 1, 0)),
			Vertex(glm::vec3(-width, 0, depth), glm::vec2(1 * texCoordScale, 1 * texCoordScale), glm::vec3(0, 1, 0)),
			Vertex(glm::vec3(width, 0, depth), glm::vec2(1 * texCoordScale, 0 * texCoordScale), glm::vec3(0, 1, 0)),
			Vertex(glm::vec3(width, 0, -depth), glm::vec2(0 * texCoordScale, 0 * texCoordScale), glm::vec3(0, 1, 0)),
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			0, 2, 3,
		};

		Mesh* mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
		return mesh;
	}
}
#endif
