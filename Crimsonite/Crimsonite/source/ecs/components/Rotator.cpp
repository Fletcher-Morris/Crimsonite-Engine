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
