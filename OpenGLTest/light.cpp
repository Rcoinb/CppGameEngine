#include "light.h"

Light::Light() {}

Light::Light(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float cutoff,
	float ambient, float range, float attenConstant, float attenLinear, float attenExponent)
{
	Set(position, direction, color, cutoff, ambient, range, attenConstant, attenLinear, attenExponent);
}

void Light::Set(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float cutoff,
	float ambient, float range, float attenConstant, float attenLinear, float attenExponent)
{
	this->l_position = position;
	this->l_direction = direction;
	this->l_color = color;

	this->l_cutoff = cutoff;
	this->l_ambient = ambient;
	this->l_range = range;
	this->l_attenConstant = attenConstant;
	this->l_attenLinear = attenLinear;
	this->l_attenExponent = attenExponent;
}

void Light::SetShadow(Shadow* shadow)
{
	this->shadow = shadow;
}

Light::~Light()
{
	delete shadow;
}