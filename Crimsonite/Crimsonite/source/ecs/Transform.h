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

	void SetPosition(glm::vec3 _newPos) { position = _newPos; }
	void SetPosition(float _x, float _y, float _z) { position = { _x,_y,_z }; }

	//	Adjust the position with another Vec3.
	void Move(glm::vec3 _move)
	{
		position.x += _move.x;
		position.y += _move.y;
		position.z += _move.z;
	}

	//	Adjust the rotation with another Vec3.
	void Rotate(glm::vec3 _rotate)
	{
		rotation.x += _rotate.x;
		rotation.y += _rotate.y;
		rotation.z += _rotate.z;
	}

private:



};