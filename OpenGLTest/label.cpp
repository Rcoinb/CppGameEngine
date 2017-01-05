#include "label.h"

Label::Label() {}

Label::Label(std::string* text, glm::vec4& color, Font* font, Display* display, float x, float y, float size, float gap)
{
	this->text = text;
	this->color = color;
	this->font = font;
	this->display = display;
	this->x = x;
	this->y = y;
	this->size = size;
	this->gap = gap;
}

Label::~Label() {}