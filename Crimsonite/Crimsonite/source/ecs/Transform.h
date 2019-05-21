#pragma once

#include <glm/common.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "../editor/EditorSerializable.h"


class Transform : public EditorSerializable
{

public:

	Transform()
	{
		position = glm::vec3();
		rotation = glm::vec3();
		localPosition = glm::vec3();
		localRotation = glm::vec3();
		m_parentTransform = NULL;
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	//	The position of the transform in world-space.
	glm::vec3 position;
	//	The rotation of the transform.
	glm::vec3 rotation;
	//	The scale of the transform.
	glm::vec3 scale;

	//	The local position of the transform in world-space.
	glm::vec3 localPosition;
	//	The local rotation of the transform.
	glm::vec3 localRotation;

	//	Return the forward vector3 from the Transform.
	glm::vec3 Forward()
	{
		forward.x = sin(glm::radians(rotation.y + localRotation.y)) * cos(glm::radians(rotation.x + localRotation.x));
		forward.y = sin(glm::radians(rotation.x + localRotation.x));
		forward.z = cos(glm::radians(rotation.y + localRotation.y)) * cos(glm::radians(rotation.x + localRotation.x));
		forward = glm::normalize(forward);
		return forward;
	}
	//	Return the backwards vector3 from the Transform.
	glm::vec3 Back()
	{
		return -Forward();
	}
	//	Return the right vector3 from the Transform.
	glm::vec3 Right()
	{
		right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), Forward()));
		return right;
	}
	//	Return the left vector3 from the Transform.
	glm::vec3 Left()
	{
		return -Right();
	}
	//	Return the up vector3 from the Transform.
	glm::vec3 Up()
	{
		up = glm::cross(Forward(), Right());
		up = glm::normalize(up);
		return up;
	}
	//	Return the down vector3 from the Transform.
	glm::vec3 Down()
	{
		return -Up();
	}

	//	Set the Transform's local position.
	void SetLocalPosition(glm::vec3 _newPos) { localPosition = _newPos; }
	void SetLocalPosition(float _x, float _y, float _z) { localPosition = { _x,_y,_z }; }
	//	Return the Transform's local position.
	glm::vec3 GetLocalPosition() { InheritParentTransformValues(); return localPosition; }

	//	Set the Transform's local rotation.
	void SetLocalRotation(glm::vec3 _newRot) { localRotation = _newRot; }
	void SetLocalRotation(float _x, float _y, float _z) { localRotation = { _x,_y,_z }; }
	//	Return the Transform's local rotation.
	glm::vec3 GetLocalRotation() { InheritParentTransformValues(); return localRotation; }

	//	Set the Transform's scale.
	void SetScale(glm::vec3 _newScale) { scale = _newScale; }
	void SetScale(float _x, float _y, float _z) { scale = { _x,_y,_z }; }
	//	Return the Transform's scale.
	glm::vec3 GetScale() { return scale; }

	//	Return the Transform's world position.
	glm::vec3 GetWorldPosition() { InheritParentTransformValues(); return position; }
	//	Return the Transform's world rotation.
	glm::vec3 GetWorldRotation() { InheritParentTransformValues(); return rotation; }

	//	Adjust the position with another Vec3.
	void Move(glm::vec3 _move)
	{
		localPosition.x += _move.x;
		localPosition.y += _move.y;
		localPosition.z += _move.z;
	}
	//	Adjust the position with another Vec3, using the forward vector.
	void Move(glm::vec3 _move, bool _useForward)
	{
		if (_useForward)
		{
			Move(Forward() * _move.z);
			Move(Right() * _move.x);
			Move(Up() * _move.y);
		}
		else
		{
			Move(_move);
		}
	}
	//	Adjust the position with three floats.
	void Move(float _x, float _y, float _z)
	{
		localPosition.x += _x;
		localPosition.y += _y;
		localPosition.z += _z;
	}

	//	Adjust the rotation with another Vec3.
	void Rotate(glm::vec3 _rotate)
	{
		localRotation.x += _rotate.x;
		localRotation.y += _rotate.y;
		localRotation.z += _rotate.z;
	}
	//	Adjust the rotation with three floats.
	void Rotate(float _x, float _y, float _z)
	{
		localRotation.x += _x;
		localRotation.y += _y;
		localRotation.z += _z;
	}

	//	Return the Transfom's current parent Transform.
	Transform * GetParent()
	{
		return m_parentTransform;
	}
	//	Set the Transform's parent Transform, maintaining it's current world position & rotation.
	void SetParent(Transform * _newParentTransform)
	{
		SetParent(_newParentTransform, true);
	}
	//	Set the Transform's parent Transform.
	void SetParent(Transform * _newParentTransform, bool _maintainValues)
	{
		if (_newParentTransform == NULL)
		{
			return;
		}
		else
		{
			if (_maintainValues)
			{
				localPosition.x -= _newParentTransform->position.x;
				localPosition.y -= _newParentTransform->position.y;
				localPosition.z -= _newParentTransform->position.z;
				localRotation.x -= _newParentTransform->rotation.x;
				localRotation.y -= _newParentTransform->rotation.y;
				localRotation.z -= _newParentTransform->rotation.z;
			}
			if (m_parentTransform != _newParentTransform)
			{
				m_parentTransform = _newParentTransform;
			}
		}
	}
	//	Make the transform independant from it's current parent, maintaining it's current world position & rotation.
	void Unparent()
	{
		Unparent(true);
	}
	//	Make the transform independant from it's current parent.
	void Unparent(bool _maintainValues)
	{
		if (m_parentTransform != NULL)
		{
			InheritParentTransformValues();
			localPosition = position;
			localRotation = rotation;
			std::cout << "Unparented entity" << std::endl;
		}
		m_parentTransform = NULL;
	}

	//	Inherit the Transform values from the parent.
	void InheritParentTransformValues()
	{
		if (m_parentTransform == NULL)
		{
			position = localPosition;
			rotation = localRotation;
		}
		else
		{
			position = m_parentTransform->position + localPosition;
			rotation = m_parentTransform->rotation + localRotation;
		}
	}

	//	Serialize the Transform's properties to a string.
	std::string Serialize() override
	{
		std::string serialized = "val ";
		serialized += std::to_string(GetLocalPosition().x);
		serialized += "\nval ";
		serialized += std::to_string(GetLocalPosition().y);
		serialized += "\nval ";
		serialized += std::to_string(GetLocalPosition().z);
		serialized += "\nval ";
		serialized += std::to_string(GetLocalRotation().x);
		serialized += "\nval ";
		serialized += std::to_string(GetLocalRotation().y);
		serialized += "\nval ";
		serialized += std::to_string(GetLocalRotation().z);
		serialized += "\nval ";
		serialized += std::to_string(GetScale().x);
		serialized += "\nval ";
		serialized += std::to_string(GetScale().y);
		serialized += "\nval ";
		serialized += std::to_string(GetScale().z);
		return serialized;
	}
	//	Deserialise the Transform from the proveded string vector.
	void Deserialize(std::vector<std::string> _data) override
	{
		SetLocalPosition(std::stof(_data[3]), std::stof(_data[4]), std::stof(_data[5]));
		SetLocalRotation(std::stof(_data[6]), std::stof(_data[7]), std::stof(_data[8]));
		SetScale(std::stof(_data[9]), std::stof(_data[10]), std::stof(_data[11]));
	}
	//	Draw the editor properties for the Transform.
	void DrawEditorProperties()
	{
		ImGui::Text("Transform Properties");
		ImGui::NewLine();
		float pos[3];
		pos[0] = GetLocalPosition().x;
		pos[1] = GetLocalPosition().y;
		pos[2] = GetLocalPosition().z;
		ImGui::DragFloat3("Position", pos, 0.05f);
		SetLocalPosition(glm::vec3{ pos[0],pos[1],pos[2] });
		ImGui::Text("World Position : %f, %f, %f", position.x, position.y, position.z);
		float rot[3];
		rot[0] = GetLocalRotation().x;
		rot[1] = GetLocalRotation().y;
		rot[2] = GetLocalRotation().z;
		ImGui::DragFloat3("Rotation", rot, 0.25f);
		SetLocalRotation(glm::vec3{ rot[0],rot[1],rot[2] });
		ImGui::Text("World Rotation : %f, %f, %f", rotation.x, rotation.y, rotation.z);
		float scale[3];
		scale[0] = GetScale().x;
		scale[1] = GetScale().y;
		scale[2] = GetScale().z;
		ImGui::DragFloat3("Scale", scale, 0.1f);
		SetScale(glm::vec3{ scale[0],scale[1],scale[2] });
	}

private:

	//	The saved forward vector from the transform's perspective.
	glm::vec3 forward;
	//	The saved right vector from the transform's perspective.
	glm::vec3 right;
	//	The saved up vector from the transform's perspective.
	glm::vec3 up;

	//	The Transform's parent Transform, if null, uses origin.
	Transform * m_parentTransform;

};