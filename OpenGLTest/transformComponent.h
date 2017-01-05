#ifndef TRANSFORMCOMPONENT_INCLUDED_H
#define TRANSFORMCOMPONENT_INCLUDED_H

#include "objectComponent.h"
#include "transform.h"

class TransformComponent : public ObjectComponent
{
public:
	TransformComponent(Transform transform)
	{
		this->transform = transform;
	}

	static ObjectComponentType* GetStaticType()
	{
		static ObjectComponentType type({ "Transform" });
		return &type;
	}

	inline virtual ObjectComponentType* GetType() const override { return GetStaticType(); }

	void Clear() {}

	inline Transform* GetTransform() { return &transform; }
private:
	Transform transform;
};

#endif