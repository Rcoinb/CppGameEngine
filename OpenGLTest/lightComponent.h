#ifndef LIGHTCOMPONENT_INCLUDED_H
#define LIGHTCOMPONENT_INCLUDED_H

#include "objectComponent.h"
#include "light.h"

class LightComponent : public ObjectComponent
{
public:
	LightComponent(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float cutoff,
		float ambient, float range, float attenConstant, float attenLinear, float attenExponent)
	{
		light = new Light(position, direction, color, cutoff, ambient, range, attenConstant, attenLinear, attenExponent);
	}

	static ObjectComponentType* GetStaticType()
	{
		static ObjectComponentType type({ "Light" });
		return &type;
	}

	inline virtual ObjectComponentType* GetType() const override { return GetStaticType(); }

	void Clear() 
	{
		delete light;
	}

	inline Light* GetLight() { return light; }
private:
	Light* light;
};

#endif