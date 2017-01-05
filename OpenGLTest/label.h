#ifndef LABEL_INCLUDED_H
#define LABEL_INCLUDED_H

#include <string>
#include <glm\glm.hpp>
#include "font.h"
#include "mesh.h"

class Label
{
public:
	Label();
	Label(std::string* text, glm::vec4& color, Font* font, Display* display, float x, float y, float size, float gap = 0);

	Font* font = nullptr;
	std::string* text;
	glm::vec4 color;
	Display* display;

	float x;
	float y;
	float size;
	float gap;

	~Label();
protected:
private:
};

#endif