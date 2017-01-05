#ifndef MESHCOMPONENT_INCLUDED_H
#define MESHCOMPONENT_INCLUDED_H

#include "objectComponent.h"
#include "mesh.h"

class MeshComponent : public ObjectComponent
{
public:
	MeshComponent(Mesh* mesh)
	{
		this->mesh = mesh;
	}

	static ObjectComponentType* GetStaticType()
	{
		static ObjectComponentType type({ "Mesh" });
		return &type;
	}

	inline virtual ObjectComponentType* GetType() const override { return GetStaticType(); }

	void Clear() {}

	inline Mesh* GetMesh() { return mesh; }
private:
	Mesh* mesh;
};

#endif