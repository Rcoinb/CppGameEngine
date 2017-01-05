#include "renderer.h"

Renderer::Renderer(Display* display)
{
	this->display = display;

	nullvec2 = new glm::vec2(NULL, NULL);
	nullvec3 = new glm::vec3(NULL, NULL, NULL);
	nullvec4 = new glm::vec4(NULL, NULL, NULL, NULL);

	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, 0), glm::vec2(1, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(-1, 1, 0), glm::vec2(0, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(1, 1, 0), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(1, -1, 0), glm::vec2(1, 1), glm::vec3(0, 0, -1)),
	};

	unsigned int indices[] = {
		3, 2, 0,
		2, 1, 0,
	};

	guimesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

	defaultShader = new Shader("./res/shaders/basicShader.vs", "./res/shaders/basicShader.fs", nullptr);
	shadowShader = new Shader("./res/shaders/shadowShader.vs", "./res/shaders/shadowShader.fs", "./res/shaders/shadowShader.gs");
	guiShader = new Shader("./res/shaders/guiShader.vs", "./res/shaders/guiShader.fs", nullptr);
	fontShader = new Shader("./res/shaders/fontShader.vs", "./res/shaders/fontShader.fs", nullptr);
}

Renderer::~Renderer()
{
	delete defaultShader;
	delete shadowShader;
	delete guiShader;
	delete fontShader;
	delete guimesh;
	delete nullvec2;
	delete nullvec3;
	delete nullvec4;
}

void Renderer::RenderMesh(Mesh& mesh, Material& material, const Transform& trans)
{
	defaultShader->Use();
	glActiveTexture(GL_TEXTURE0 + 31);
	glBindTexture(GL_TEXTURE_2D, material.texture->m_texture);
	glUniform1i(glGetUniformLocation(defaultShader->Program, "sampler"), 31);
	glUniform4f(glGetUniformLocation(defaultShader->Program, "mat_color"), material.color.x, material.color.y, material.color.z, material.color.w);
	glUniform1f(glGetUniformLocation(defaultShader->Program, "mat_specularIntensity"), material.specularIntensity);
	glUniform1f(glGetUniformLocation(defaultShader->Program, "mat_specularPower"), material.specularPower);
	glUniformMatrix4fv(glGetUniformLocation(defaultShader->Program, "modelMatrix"), 1, GL_FALSE, &trans.modelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(defaultShader->Program, "normalMatrix"), 1, GL_FALSE, &trans.rotMatrix[0][0]);
	mesh.Draw();
}

void Renderer::RenderObject(Object* object)
{
	this->RenderMesh(*object->GetComponent<MeshComponent>()->GetMesh(), *object->GetComponent<MaterialComponent>()->GetMaterial(), *object->GetComponent<TransformComponent>()->GetTransform());
}

void Renderer::RenderScene(Scene* scene)
{
	defaultShader->Use();

	unsigned int lights = 0;

	for (unsigned int i = 0; i < scene->GetObjects()->size(); i++)
	{
		if (scene->GetObjects()->at(i)->GetComponent<LightComponent>() != nullptr)
		{
			Light* light = scene->GetObjects()->at(i)->GetComponent<LightComponent>()->GetLight();

			if (light->GetShadow() != nullptr)
			{
				light->GetShadow()->PrepareShadow(*shadowShader, *light->GetPosition());
				for (unsigned int ii = 0; ii < scene->GetObjects()->size(); ii++)
				{
					if (scene->GetObjects()->at(ii)->GetComponent<MeshComponent>() != nullptr)
						light->GetShadow()->RenderToShadow(*shadowShader, *scene->GetObjects()->at(ii)->GetComponent<MeshComponent>()->GetMesh(), *scene->GetObjects()->at(ii)->GetComponent<TransformComponent>()->GetTransform(), *light->GetPosition());
				}

				defaultShader->Use();
				glActiveTexture(GL_TEXTURE0 + lights);
				glBindTexture(GL_TEXTURE_CUBE_MAP, light->GetShadow()->depthCubemap);
				glUniform1i(glGetUniformLocation(defaultShader->Program, ("s_cubeMap[" + std::to_string(lights) + "]").c_str()), lights);
				glUniform1f(glGetUniformLocation(defaultShader->Program, ("s_fplane[" + std::to_string(lights) + "]").c_str()), light->GetShadow()->FAR_PLANE);
			}

			glUniform3f(glGetUniformLocation(defaultShader->Program, ("l_position[" + std::to_string(lights) + "]").c_str()), light->GetPosition()->x, light->GetPosition()->y, light->GetPosition()->z);
			glUniform3f(glGetUniformLocation(defaultShader->Program, ("l_direction[" + std::to_string(lights) + "]").c_str()), light->GetDirection()->x, light->GetDirection()->y, light->GetDirection()->z);
			glUniform3f(glGetUniformLocation(defaultShader->Program, ("l_color[" + std::to_string(lights) + "]").c_str()), light->GetColor()->x, light->GetColor()->y, light->GetColor()->z);

			glUniform1f(glGetUniformLocation(defaultShader->Program, ("l_cutoff[" + std::to_string(lights) + "]").c_str()), *light->GetCutoff());
			glUniform1f(glGetUniformLocation(defaultShader->Program, ("l_ambient[" + std::to_string(lights) + "]").c_str()), *light->GetAmbient());
			glUniform1f(glGetUniformLocation(defaultShader->Program, ("l_range[" + std::to_string(lights) + "]").c_str()), *light->GetRange());
			glUniform1f(glGetUniformLocation(defaultShader->Program, ("l_attenConstant[" + std::to_string(lights) + "]").c_str()), *light->GetAttenConstant());
			glUniform1f(glGetUniformLocation(defaultShader->Program, ("l_attenLinear[" + std::to_string(lights) + "]").c_str()), *light->GetAttenLinear());
			glUniform1f(glGetUniformLocation(defaultShader->Program, ("l_attenExponent[" + std::to_string(lights) + "]").c_str()), *light->GetAttenExponent());

			lights++;
		}
	}

	glUniform1i(glGetUniformLocation(defaultShader->Program, "l_count"), lights);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, display->GetWidth(), display->GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	defaultShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(defaultShader->Program, "projectionMatrix"), 1, GL_FALSE, &scene->GetCamera()->GetProjectionMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(defaultShader->Program, "viewMatrix"), 1, GL_FALSE, &scene->GetCamera()->GetViewMatrix()[0][0]);

	for (int i = 0; i < scene->GetObjects()->size(); i++)
	{
		Object* object = scene->GetObjects()->at(i);
		if (scene->GetObjects()->at(i)->GetComponent<MeshComponent>() != nullptr)
		{
			if (scene->GetCamera()->GetFrustum()->CubeInFrustum(*object->GetComponent<TransformComponent>()->GetTransform()->GetPos(), *object->GetComponent<MeshComponent>()->GetMesh()->GetAABB()))
				this->RenderObject(scene->GetObjects()->at(i));
		}
	}
}

void Renderer::RenderGui(int texture, const glm::mat4& mat, const glm::vec4& color, int useSubTex, float imageDim, const glm::vec2& subTexCor, const glm::vec2& subTexDim)
{
	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	guiShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 1);
	glUniform1i(glGetUniformLocation(guiShader->Program, "guiTexture"), 0);

	glUniform4f(glGetUniformLocation(guiShader->Program, "color"), color.x, color.y, color.z, color.w);

	glUniform1i(glGetUniformLocation(guiShader->Program, "useSubTex"), useSubTex);
	glUniform1f(glGetUniformLocation(guiShader->Program, "imageDim"), imageDim);
	if (&subTexCor != nullvec2)
		glUniform2f(glGetUniformLocation(guiShader->Program, "subTexCor"), subTexCor.x, subTexCor.y);
	if (&subTexDim != nullvec2)
		glUniform2f(glGetUniformLocation(guiShader->Program, "subTexDim"), subTexDim.x, subTexDim.y);

	glUniformMatrix4fv(glGetUniformLocation(guiShader->Program, "modelMatrix"), 1, GL_FALSE, &mat[0][0]);
	guimesh->Draw();

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void Renderer::RenderString(const std::string& text, float x, float y, float size, float gap, const glm::vec4& color, Font& font)
{
	float curX = 0;

	int letters = 0;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	float verticalPerPixelSize = 0.005 / (double) font.lineHeight;
	float horizontalPerPixelSize = verticalPerPixelSize / display->GetAspect();

	for (unsigned int i = 0; i < text.length(); i++) {
		chr c = font.chrs[(int)text.c_str()[i]];

		glm::vec3 position[] = {
			glm::vec3(c.xoffset + curX, c.yoffset, 0),
			glm::vec3(c.xoffset + curX, c.height + c.yoffset, 0),
			glm::vec3(c.width + c.xoffset + curX, c.height + c.yoffset, 0),
			glm::vec3(c.width + c.xoffset + curX, c.yoffset, 0)
		};

		


		glm::vec2 texCoords[] = {
			glm::vec2(((float) c.x / (float) font.textureDim), ((float) c.y / (float) font.textureDim)),
			glm::vec2(((float) c.x / (float) font.textureDim), ((float) (c.y + c.height) / (float) font.textureDim)),
			glm::vec2(((float) (c.x + c.width) / (float) font.textureDim), ((float) (c.y + c.height) / (float) font.textureDim)),
			glm::vec2(((float) (c.x + c.width) / (float) font.textureDim), ((float) c.y / (float) font.textureDim))
		};

		for (unsigned int i = 0; i < 4; i++)
			vertices.push_back(Vertex(position[i], texCoords[i], glm::vec3(0, 0, 0)));

		indices.push_back(0 + letters * 4);
		indices.push_back(1 + letters * 4);
		indices.push_back(2 + letters * 4);
		indices.push_back(0 + letters * 4);
		indices.push_back(2 + letters * 4);
		indices.push_back(3 + letters * 4);

		curX = curX + c.xadvance + gap;
		letters++;
	}

	Mesh textMesh(&vertices.at(0), vertices.size(), &indices.at(0), indices.size());

	glm::mat4 mat;
	mat = glm::translate(glm::vec3(x, y, 0)) * glm::scale(glm::vec3(size * horizontalPerPixelSize, -size * verticalPerPixelSize, 0)); //FLIPPING Y-AXIS, NOT GOOD APPROACH?

	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fontShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font.tex->m_texture);
	glUniform1i(glGetUniformLocation(fontShader->Program, "fontTexture"), 0);


	glUniformMatrix4fv(glGetUniformLocation(fontShader->Program, "modelMatrix"), 1, GL_FALSE, &mat[0][0]);
	glUniform4f(glGetUniformLocation(fontShader->Program, "color"), color.x, color.y, color.z, color.w);
	textMesh.Draw();

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void Renderer::RenderString(Label* label)
{
	RenderString(*label->text, label->x, label->y, label->size, label->gap, label->color, *label->font);
}

void Renderer::RenderGui(int texture, const Transform& trans, const glm::vec4& color, int useSubTex, float imageDim, const glm::vec2& subTexCor, const glm::vec2& subTexDim)
{
	RenderGui(texture, trans.modelMatrix, color, useSubTex, imageDim, subTexCor, subTexDim);
}

void Renderer::RenderGui(int texture, const Transform& trans, const glm::vec4& color)
{
	RenderGui(texture, trans, color, 0, 0, *nullvec2, *nullvec2);
}