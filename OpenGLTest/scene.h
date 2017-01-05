#ifndef SCENE_INCLUDED_H
#define SCENE_INCLUDED_H

#include <iostream>
#include <GL/glew.h>
#include "mesh.h"
#include "object.h"
#include "shader.h"
#include "camera.h"
#include "light.h"
#include "sceneComponent.h"
#include "physicsWorldComponent.h"

class Scene
{
public:
	Scene();

	Object* CreateObject();

	void DeleteObject(Object* object);

	void SetCamera(Camera* camera);

	void ClearScene();

	void Update();

	inline Camera* GetCamera() { return camera; }

	inline std::vector<Object*>* GetObjects() { return &objects; }

	void AddComponent(SceneComponent* c);

	void ClearComponents();

	template <typename T>
	inline T* GetComponent()
	{
		SceneComponentType* type = T::GetStaticType();
		for (auto x : components)
		{
			if (x->GetType() == type)
				return (T*)x;
		}
		return nullptr;
	}

	virtual ~Scene();
protected:
private:
	Camera* camera;

	std::vector<Object*> objects;
	std::vector<SceneComponent*> components;
};

#endif
