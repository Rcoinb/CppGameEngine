#ifndef SCENECOMPONENT_INCLUDED_H
#define SCENECOMPONENT_INCLUDED_H

#include <string>;

struct SceneComponentType
{
	std::string name;
};

class SceneComponent
{
public:
	virtual SceneComponentType* GetType() const { return nullptr; }

	virtual void Clear() = 0;
};

#endif