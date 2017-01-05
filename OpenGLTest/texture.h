#ifndef TEXTURE_INCLUDED_H
#define TEXTURE_INCLUDED_H

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture(const std::string& fileName, bool mipmap = true);

	virtual ~Texture();
	GLuint m_texture;
protected:
private:
};

#endif
