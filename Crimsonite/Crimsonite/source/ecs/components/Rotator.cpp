#include "Rotator.h"

void Rotator::OnInit()
{
	m_axis = { 0.2f, 0.2f, 0.0f };
}

void Rotator::OnUpdate()
{
	entity->transform.Rotate(m_axis * Time::DeltaTime());
}

void Rotator::OnFixedUpdate()
{
}

void Rotator::OnRender()
{
}

void Rotator::OnEnable()
{
}

void Rotator::OnDisable()
{
}

void Rotator::DrawEditorProperties()
{
	float newAxis[3];
	newAxis[0] = GetAxis().x;
	newAxis[1] = GetAxis().y;
	newAxis[2] = GetAxis().z;
	ImGui::DragFloat3("Axis", newAxis);
	SetAxis(glm::vec3{ newAxis[0],newAxis[1],newAxis[2] });
}

std::string Rotator::Serialize()
{
	std::string serialized = "val " + std::to_string(m_axis.x);
	serialized += "\nval " + std::to_string(m_axis.y);
	serialized += "\nval " + std::to_string(m_axis.z);
	return serialized;
}

void Rotator::Deserialize(std::vector<std::string> _data)
{
	//	_data[0] is just the component name.
	//	_data[1] is just the enabled state.
	SetAxis(glm::vec3(std::stof(_data[2]), std::stof(_data[3]), std::stof(_data[4])));
}
