#ifndef FONT_INCLUDED_H
#define FONT_INCLUDED_H

#include <string>
#include <glm\glm.hpp>
#include <list>
#include <fstream>
#include <vector>
#include <map>
#include "texture.h"
#include "display.h"
#include "transform.h"

struct chr
{
public:
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;
};

class Font
{
public:
	Font(const std::string& filePath, Texture& fontTex, int textureDim, int lineHeight);

	Texture* tex;
	int textureDim;
	int lineHeight;

	std::map<int, chr> chrs;

	~Font();
protected:
private:
	std::string DATA_IN_LINE[8] = {"id=", "x=", "y=", "width=", "height=", "xoffset=", "yoffset=", "xadvance="};

	Transform trans;
};

#endif