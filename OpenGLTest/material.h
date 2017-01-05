#ifndef MATERIAL_INCLUDED_H
#define MATERIAL_INCLUDED_H

#include <glm\glm.hpp>
#include "texture.h"

class Material
{
public:
	Material(Texture* texture, glm::vec4& color = glm::vec4(0, 0, 0, 0), float sIntensity = 0, float sPower = 0)
	{
		this->texture = texture;
		this->color = color;
		this->specularIntensity = sIntensity;
		this->specularPower = sPower;
	}

	Texture* texture;
	glm::vec4 color;
	float specularIntensity;
	float specularPower;

	~Material() {}
private:
};

#endif