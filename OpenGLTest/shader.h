#ifndef SHADER_INCLUDED_H
#define SHADER_INCLUDED_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	GLuint Program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);

	void Use();

	~Shader();
protected:
private:
	void checkCompileErrors(GLuint shader, std::string type);
};

#endif