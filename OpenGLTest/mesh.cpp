#include "mesh.h"
#include "util.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>

Mesh::Mesh() {}

Mesh::Mesh(const std::string& fileName)
{
	Init(fileName);
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	Init(vertices, numVertices, indices, numIndices);
}

void Mesh::Init(const std::string& fileName)
{
    InitMesh(OBJModel(fileName).ToIndexedModel());
}

void Mesh::Init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel model;

	for (unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
	}

	for (unsigned int i = 0; i < numIndices; i++)
		model.indices.push_back(indices[i]);

	InitMesh(model);
}

void Mesh::InitMesh(const IndexedModel& model)
{
	btConvexHullShape* convexShape = new btConvexHullShape();

	glm::vec3 v1 = model.positions[0];
	glm::vec3 v2 = model.positions[0];

	for (unsigned int i = 0; i < model.positions.size(); i++)
	{
		convexShape->addPoint(btVector3(model.positions[i].x, model.positions[i].y, model.positions[i].z));

		if (model.positions[i].x > v1.x) v1.x = model.positions[i].x;
		if (model.positions[i].y > v1.y) v1.y = model.positions[i].y;
		if (model.positions[i].z > v1.z) v1.z = model.positions[i].z;
		
		if (model.positions[i].x < v2.x) v2.x = model.positions[i].x;
		if (model.positions[i].y < v2.y) v2.y = model.positions[i].y;
		if (model.positions[i].z < v2.z) v2.z = model.positions[i].z;

	}

	aabb.SetAABB(v1, v2);

	collisionShape = convexShape;

    m_numIndices = model.indices.size();

    glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
	if (collisionShape != nullptr)
		delete collisionShape;
}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);

	//glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	//glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
