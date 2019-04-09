#pragma once

#include <glm/common.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>
#include <string>


class Transform
{

public:

	Transform()
	{
		position = glm::vec3();
		rotation = glm::vec3();
		scale = glm::vec3(1.0f,1.0f,1.0f);
	}

	//	The position of this transform in world-space.
	glm::vec3 position;
	//	The rotation of this transform.
	glm::vec3 rotation;
	//	The scale of this transform.
	glm::vec3 scale;

	glm::vec3 Forward()
	{
		forward.x = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		forward.y = sin(glm::radians(rotation.x));
		forward.z = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		forward = glm::normalize(forward);
		return forward;
	}
	glm::vec3 Back()
	{
		return -Forward();
	}
	glm::vec3 Right()
	{
		right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), Forward()));
		return right;
	}
	glm::vec3 Left()
	{
		return -Right();
	}
	glm::vec3 Up()
	{
		up = glm::cross(Forward(), Right());
		up = glm::normalize(up);
		return up;
	}
	glm::vec3 Down()
	{
		return -Up();
	}

	void SetPosition(glm::vec3 _newPos) { position = _newPos; }
	void SetPosition(float _x, float _y, float _z) { position = { _x,_y,_z }; }
	glm::vec3 GetPosition() { return position; }

	void SetRotation(glm::vec3 _newRot) { rotation = _newRot; }
	void SetRotation(float _x, float _y, float _z) { rotation = { _x,_y,_z }; }
	glm::vec3 GetRotation() { return rotation; }

	void SetScale(glm::vec3 _newScale) { scale = _newScale; }
	void SetScale(float _x, float _y, float _z) { scale = { _x,_y,_z }; }
	glm::vec3 GetScale() { return scale; }

	//	Adjust the position with another Vec3.
	void Move(glm::vec3 _move)
	{
		position.x += _move.x;
		position.y += _move.y;
		position.z += _move.z;
	}
	//	Adjust the position with three floats.
	void Move(float _x, float _y, float _z)
	{
		position.x += _x;
		position.y += _y;
		position.z += _z;
	}

	//	Adjust the rotation with another Vec3.
	void Rotate(glm::vec3 _rotate)
	{
		rotation.x += _rotate.x;
		rotation.y += _rotate.y;
		rotation.z += _rotate.z;
	}
	//	Adjust the rotation with three floats.
	void Rotate(float _x, float _y, float _z)
	{
		rotation.x += _x;
		rotation.y += _y;
		rotation.z += _z;
	}

	std::string Serialize()
	{
		std::string serialized = "val ";
		serialized += std::to_string(GetPosition().x);
		serialized += "\nval ";
		serialized += std::to_string(GetPosition().y);
		serialized += "\nval ";
		serialized += std::to_string(GetPosition().z);
		serialized += "\nval ";
		serialized += std::to_string(GetRotation().x);
		serialized += "\nval ";
		serialized += std::to_string(GetRotation().y);
		serialized += "\nval ";
		serialized += std::to_string(GetRotation().z);
		return serialized;
	}
	void Deserialize(std::vector<std::string> _data)
	{
		SetPosition(std::stof(_data[3]), std::stof(_data[4]), std::stof(_data[5]));
		SetRotation(std::stof(_data[6]), std::stof(_data[7]), std::stof(_data[8]));
	}

private:

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

};