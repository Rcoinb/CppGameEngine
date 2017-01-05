#ifndef FRUSTUM_INCLUDED_H
#define FRUSTUM_INCLUDED_H

#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "AABB.h"

struct Frustum
{
public:
	Frustum();

	void CalculateFrustum(const glm::mat4& projMatrix, const glm::mat4& viewMatrix);
	bool PointInFrustum(float x, float y, float z);
	bool SphereInFrustum(float x, float y, float z, float radius);
	bool CubeInFrustum(float x, float y, float z, float width, float height, float depth);
	bool CubeInFrustum(const glm::vec3& pos, const glm::vec3& dim);
	bool CubeInFrustum(glm::vec3& pos, AABB& aabb);

	~Frustum();
protected:
private:
	float m_Frustum[6][4];
};

#endif
