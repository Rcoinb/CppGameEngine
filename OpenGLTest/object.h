#ifndef OBJECT_INCLUDED_H
#define OBJECT_INCLUDED_H

#include <iostream>
#include "objectComponent.h"
#include "meshComponent.h"
#include "materialComponent.h"
#include "transformComponent.h"
#include "rigidBodyComponent.h"
#include "lightComponent.h"

class Object
{
public:
	Object() {}

	void AddComponent(ObjectComponent* c)
	{
		components.push_back(c);
	}

	void ClearComponents()
	{
		for (auto i : components)
		{
			i->Clear();
			delete i;
		}
		components.clear();
	}

	template <typename T>
	inline T* GetComponent()
	{
		ObjectComponentType* type = T::GetStaticType();
		for (auto x : components)
		{
			if (x->GetType() == type)
				return (T*)x;
		}
		return nullptr;
	}

	virtual ~Object() {}
protected:
private:
	std::vector<ObjectComponent*> components;
};

#endif
