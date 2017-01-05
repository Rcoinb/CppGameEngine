#ifndef MATERIALCOMPONENT_INCLUDED_H
#define MATERIALCOMPONENT_INCLUDED_H

#include "objectComponent.h"
#include "material.h"

class MaterialComponent : public ObjectComponent
{
public:
	MaterialComponent(Material* material)
	{
		this->material = material;
	}

	static ObjectComponentType* GetStaticType()
	{
		static ObjectComponentType type({ "Material" });
		return &type;
	}

	inline virtual ObjectComponentType* GetType() const override { return GetStaticType(); }

	void Clear() {}

	inline Material* GetMaterial() { return material; }
private:
	Material* material;
};

#endif