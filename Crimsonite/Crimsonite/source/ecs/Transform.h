#pragma once

#include <glm/common.hpp>
#include <glm/vec3.hpp>

class Transform
{

public:

	Transform()
	{
		position = glm::vec3();
		rotation = glm::vec3();
	}

	//	The position of this transform in world-space.
	glm::vec3 position;
	//	The rotation of this transform.
	glm::vec3 rotation;

private:



};