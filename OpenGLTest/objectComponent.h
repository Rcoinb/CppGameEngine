#ifndef OBJECTCOMPONENT_INCLUDED_H
#define OBJECTCOMPONENT_INCLUDED_H

#include <string>;

struct ObjectComponentType
{
	std::string name;
};

class ObjectComponent
{
public:
	virtual ObjectComponentType* GetType() const { return nullptr; }

	virtual void Clear() = 0;
};

#endif