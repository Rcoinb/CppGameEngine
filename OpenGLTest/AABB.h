#ifndef AABB_H_INCLUDED
#define AABB_H_INCLUDED

#include <glm\glm.hpp>

class AABB
{
public:
	AABB() {}

	AABB(glm::vec3& v1, glm::vec3& v2)
	{
		SetAABB(v1, v2);
	}

	void SetAABB(glm::vec3& v1, glm::vec3& v2)
	{
		this->v1 = v1;
		this->v2 = v2;
	}

	void ApplyTransformation(glm::mat4& modelMatrix)
	{
		glm::vec4 v1vec4 = modelMatrix * glm::vec4(v1, 0);
		glm::vec4 v2vec4 = modelMatrix * glm::vec4(v2, 0);

		transv1 = glm::vec3(v1vec4.x, v1vec4.y, v1vec4.z);
		transv2 = glm::vec3(v2vec4.x, v2vec4.y, v2vec4.z);
	}

	inline glm::vec3* GetV1() { return &v1; }
	inline glm::vec3* GetV2() { return &v2; }

	inline glm::vec3* GetTransV1() { return &transv1; }
	inline glm::vec3* GetTransV2() { return &transv2; }
private:
	glm::vec3 v1;
	glm::vec3 v2;

	glm::vec3 transv1;
	glm::vec3 transv2;
};

#endif
