#ifndef PHYSICSWORLDCOMPONENT_INCLUDED_H
#define PHYSICWORLDCOMPONENT_INCLUDED_H

#include "sceneComponent.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class PhysicsWorldComponent : public SceneComponent
{
public:
	PhysicsWorldComponent(const btVector3& gravity)
	{
		broadphase = new btDbvtBroadphase();
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		solver = new btSequentialImpulseConstraintSolver;
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		dynamicsWorld->setGravity(gravity);
	}

	static SceneComponentType* GetStaticType()
	{
		static SceneComponentType type({ "PhysicsWorld" });
		return &type;
	}

	inline virtual SceneComponentType* GetType() const override { return GetStaticType(); }

	inline btDynamicsWorld* GetDynamicsWorld() { return dynamicsWorld; }

	void Clear() override
	{
		delete dynamicsWorld;
		delete solver;
		delete collisionConfiguration;
		delete dispatcher;
		delete broadphase;
	}
private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif