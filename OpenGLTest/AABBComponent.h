#ifndef AABBCOMPONENT_INCLUDED_H
#define AABBCOMPONENT_INCLUDED_H

#include "objectComponent.h"
#include "AABB.h"

class AABBComponent : public ObjectComponent
{
public:
	AABBComponent(AABB* aabb)
	{
		this->aabb = aabb;
	}

	static ComponentType* GetStaticType()
	{
		static ComponentType type({ "AABB" });
		return &type;
	}

	inline virtual ComponentType* GetType() const override { return GetStaticType(); }

	AABB* GetAABB() { return aabb; }
private:
	AABB* aabb;
};

#endif