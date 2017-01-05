#ifndef RENDERER_INCLUDED_H
#define RENDERER_INCLUDED_H

#include <glm\glm.hpp>
#include <string>
#include "mesh.h"
#include "display.h"
#include "shader.h"
#include "font.h"
#include "material.h"
#include "transform.h"
#include "object.h"
#include "scene.h"
#include "label.h"

class Renderer
{
public:
	Renderer(Display* display);

	void RenderMesh(Mesh& mesh, Material& material, const Transform& trans);

	void RenderScene(Scene* scene);

	void RenderObject(Object* object);

	void RenderGui(int texture, const glm::mat4& mat, const glm::vec4& color, int useSubTex, float imageDim, const glm::vec2& subTexCor, const glm::vec2& subTexDim);
	void RenderGui(int texture, const Transform& trans, const glm::vec4& color, int useSubTex, float imageDim, const glm::vec2& subTexCor, const glm::vec2& subTexDim);
	void RenderGui(int texture, const Transform& trans, const glm::vec4& color);
	

	void RenderString(const std::string& text, float x, float y, float size, float gap, const glm::vec4& color, Font& font);
	void RenderString(Label* label);

	Shader* defaultShader;
	Shader* guiShader;
	Shader* fontShader;
	Shader* shadowShader;

	~Renderer();
protected:
private:
	const glm::vec2* nullvec2;
	const glm::vec3* nullvec3;
	const glm::vec4* nullvec4;
	

	Display* display;
	Mesh* guimesh;
};

#endif