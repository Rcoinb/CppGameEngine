#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "frustum.h"

struct Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		this->pos = pos;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, zNear, zFar);
	}

	void SetPosition(glm::vec3& pos) 
	{
		this->pos = pos;
	}

	void SetForward(glm::vec3& forward)
	{
		this->forward = forward;
	}

	void SetUp(glm::vec3& up)
	{
		this->up = up;
	}

	inline glm::mat4 GetProjectionMatrix() const
	{
		return projection;
	}

	inline glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(pos, pos + forward, up);
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	inline glm::vec3* GetPos() { return &pos; }

	inline Frustum* GetFrustum() { return &frustum; }

protected:
private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;

	Frustum frustum;
};

#endif
