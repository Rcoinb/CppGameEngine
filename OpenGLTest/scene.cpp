#include "scene.h"


Scene::Scene()
{
}

Scene::~Scene()
{
	ClearScene();
}

void Scene::SetCamera(Camera* camera)
{
	this->camera = camera;
}

Object* Scene::CreateObject()
{
	Object* object = new Object();
	objects.push_back(object);
	return object;
}

void Scene::AddComponent(SceneComponent* c)
{
	components.push_back(c);
}

void Scene::Update()
{
	if (GetComponent<PhysicsWorldComponent>() != nullptr)
		GetComponent<PhysicsWorldComponent>()->GetDynamicsWorld()->stepSimulation(1 / 20.0f, 10);

	for (unsigned int i = 0; i < GetObjects()->size(); i++)
	{
		Object* object = GetObjects()->at(i);
		if (object->GetComponent<RigidBodyComponent>() != nullptr)
			object->GetComponent<RigidBodyComponent>()->UpdateModelMatrix();

		if (object->GetComponent<MeshComponent>() != nullptr && object->GetComponent<TransformComponent>() != nullptr)
			object->GetComponent<MeshComponent>()->GetMesh()->GetAABB()->ApplyTransformation(object->GetComponent<TransformComponent>()->GetTransform()->modelMatrix);
	}
}

void Scene::DeleteObject(Object* object)
{
	for (unsigned int i = 0; i < objects.size(); i ++)
	{
		if (objects[i] == object)
		{
			objects.erase(objects.begin() + i);
			delete object;
		}
	}
}

void Scene::ClearComponents()
{
	for (auto i : components)
	{
		i->Clear();
		delete i;
	}
	components.clear();
}

void Scene::ClearScene()
{
	ClearComponents();

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->ClearComponents();
	    delete objects[i];
	}
	objects.clear();
}