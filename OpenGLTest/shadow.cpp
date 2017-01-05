#include "shadow.h"


Shadow::Shadow()
{
	glGenFramebuffers(1, &depthMapFBO);
	
	glGenTextures(1, &depthCubemap);

	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::PrepareShadow(Shader& shader, const glm::vec3& lightPos)
{
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (GLfloat) SHADOW_WIDTH / (GLfloat) SHADOW_HEIGHT, 1.0f, FAR_PLANE);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.Use();
	
	for (GLuint i = 0; i < 6; ++i)
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, ("shadowTransforms[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, &shadowTransforms[i][0][0]);

	glUniform1f(glGetUniformLocation(shader.Program, "far_plane"), FAR_PLANE);
	glUniform3fv(glGetUniformLocation(shader.Program, "lightPos"), 1, &lightPos[0]);
}

void Shadow::RenderToShadow(Shader& shader, Mesh& mesh, Transform& trans, const glm::vec3& lightPos)
{
	if (trans.GetPos()->x < lightPos.x + FAR_PLANE && trans.GetPos()->x > lightPos.x - FAR_PLANE
		&& trans.GetPos()->y < lightPos.y + FAR_PLANE && trans.GetPos()->y > lightPos.y - FAR_PLANE
		&& trans.GetPos()->z < lightPos.z + FAR_PLANE && trans.GetPos()->z > lightPos.z - FAR_PLANE)
	{
		glCullFace(GL_FRONT);
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, &trans.modelMatrix[0][0]);
		mesh.Draw();
		glCullFace(GL_BACK);
	}
}

Shadow::~Shadow()
{
	glDeleteTextures(1, &depthCubemap);
	glDeleteFramebuffers(1, &depthMapFBO);
}