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
