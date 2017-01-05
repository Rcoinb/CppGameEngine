#ifndef TRANSFORM_INCLUDED_H
#define TRANSFORM_INCLUDED_H

#include <glm/glm.hpp>
#include "camera.h"

struct Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->pos = pos;
		this->rot = rot;
		this->scale = scale;
	}

	void UpdateTransform()
	{
		glm::mat4 posMat = glm::translate(pos);
		glm::mat4 scaleMat = glm::scale(scale);
		glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));

		rotMatrix = rotX * rotY * rotZ;

		modelMatrix = posMat * rotMatrix * scaleMat;
	}


	glm::mat4 modelMatrix;
	glm::mat4 rotMatrix;

	inline glm::vec3* GetPos() { return &pos; }
	inline glm::vec3* GetRot() { return &rot; }
	inline glm::vec3* GetScale() { return &scale; }

	inline void SetPos(glm::vec3& pos) 
	{ 
		this->pos = pos; 
	}
	inline void SetRot(glm::vec3& rot) 
	{ 
		this->rot = rot; 
	}
	inline void SetScale(glm::vec3& scale) 
	{ 
		this->scale = scale; 
	}
protected:
private:
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
};

#endif
