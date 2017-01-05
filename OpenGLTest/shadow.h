#ifndef SHADOW_INCLUDED_H
#define SHADOW_INCLUDED_H

#include <iostream>
#include <GL/glew.h>
#include "mesh.h"
#include "transform.h"
#include "shader.h"
#include "camera.h"

class Shadow
{
public:
	Shadow();

	void PrepareShadow(Shader& shader, const glm::vec3& lightPos);
	void RenderToShadow(Shader& shader, Mesh& mesh, Transform& trans, const glm::vec3& lightPos);

	virtual ~Shadow();
	GLuint depthCubemap;
	float FAR_PLANE = 100.0;
protected:
private:
	GLuint SHADOW_WIDTH = 1024;
	GLuint SHADOW_HEIGHT = 1024;

	GLuint depthMapFBO;
};

#endif
