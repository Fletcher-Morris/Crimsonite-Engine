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

	glm::vec3 position;
	glm::vec3 rotation;

private:



};