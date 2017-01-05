#ifndef RIGIDBODYCOMPONENT_INCLUDED_H
#define RIGIDBODYCOMPONENT_INCLUDED_H

#include "objectComponent.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "transform.h"

class RigidBodyComponent : public ObjectComponent
{
public:
	RigidBodyComponent(btCollisionShape* collisionShape, Transform* transform, float mass, glm::vec3& inertia)
	{
		this->collisionShape = collisionShape;
		this->transform = transform;
		btDefaultMotionState* objectMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(transform->GetPos()->x, transform->GetPos()->y, transform->GetPos()->z)));
		btVector3 objectInertia(inertia.x, inertia.y, inertia.z);
		collisionShape->calculateLocalInertia(mass, objectInertia);
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, objectMotionState, collisionShape, objectInertia);
		rigidBody = new btRigidBody(rigidBodyCI);
	}

	void UpdateModelMatrix()
	{
		float mat[16];
		rigidBody->getWorldTransform().getOpenGLMatrix(mat);
		glm::mat4 modelMatrix = glm::make_mat4x4(mat);
		
		btQuaternion rot = rigidBody->getWorldTransform().getRotation();
		
		transform->SetPos(glm::vec3(rigidBody->getWorldTransform().getOrigin().getX(), rigidBody->getWorldTransform().getOrigin().getY(), rigidBody->getWorldTransform().getOrigin().getZ()));

		transform->rotMatrix = glm::mat4_cast(glm::quat(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));

		transform->modelMatrix = modelMatrix;
	}

	void Clear() override
	{
		delete rigidBody->getMotionState();
		delete rigidBody;
	}

	static ObjectComponentType* GetStaticType()
	{
		static ObjectComponentType type({ "RigidBody" });
		return &type;
	}

	inline virtual ObjectComponentType* GetType() const override { return GetStaticType(); }

	inline btCollisionShape* GetCollisionShape() { return collisionShape; }
	inline btRigidBody* GetRigidBody() { return rigidBody; }
private:
	btCollisionShape* collisionShape;
	btRigidBody* rigidBody;
	Transform* transform;
};

#endif