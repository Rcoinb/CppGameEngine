#ifndef LIGHT_INCLUDED_H
#define LIGHT_INCLUDED_H

#include <glm\glm.hpp>
#include <GL/glew.h>
#include "shader.h"
#include "shadow.h"

class Light
{
public:
	Light();

	Light(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float cutoff,
		float ambient, float range, float attenConstant, float attenLinear, float attenExponent);

	void Set(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float cutoff,
		float ambient, float range, float attenConstant, float attenLinear, float attenExponent);

	void SetShadow(Shadow* shadow);

	inline glm::vec3* GetPosition() { return &l_position; }
	inline glm::vec3* GetDirection() { return &l_direction; }
	inline glm::vec3* GetColor() { return &l_color; }

	inline float* GetCutoff() { return &l_cutoff; }
	inline float* GetAmbient() { return &l_ambient; }
	inline float* GetRange() { return &l_range; }
	inline float* GetAttenConstant() { return &l_attenConstant; }
	inline float* GetAttenLinear() { return &l_attenLinear; }
	inline float* GetAttenExponent() { return &l_attenExponent; }

	inline Shadow* GetShadow() { return shadow; }

	virtual ~Light();
protected:
private:
	glm::vec3 l_position;
	glm::vec3 l_direction;
	glm::vec3 l_color;

	float l_cutoff;
	float l_ambient;
	float l_range;
	float l_attenConstant;
	float l_attenLinear;
	float l_attenExponent;

	Shadow* shadow = nullptr;
};

#endif
