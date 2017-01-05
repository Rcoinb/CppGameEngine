#include "font.h"


Font::Font(const std::string& filePath, Texture& fontTex, int textureDim, int lineHeight)
{
	std::ifstream infile(filePath);
	for (std::string line; std::getline(infile, line); )
	{
		if (line.find("char ") == 0)
		{
			int id;
			chr c;
			for (int i = 0; i < (sizeof(DATA_IN_LINE) / sizeof(*DATA_IN_LINE)); i++)
			{
				std::string curData = line.substr(line.find(DATA_IN_LINE[i]));
				curData = curData.substr(DATA_IN_LINE[i].length(), curData.find(" ") - DATA_IN_LINE[i].length());
				int data = std::atoi(curData.c_str());
				if (i == 0) id = data;
				if (i == 1) c.x = data;
				if (i == 2) c.y = data;
				if (i == 3) c.width = data;
				if (i == 4) c.height = data;
				if (i == 5) c.xoffset = data;
				if (i == 6) c.yoffset = data;
				if (i == 7) c.xadvance = data;
			}
			chrs[id] = c;
		}
	}
	this->textureDim = textureDim;
	this->lineHeight = lineHeight;
	this->tex = &fontTex;
}

Font::~Font()
{
}