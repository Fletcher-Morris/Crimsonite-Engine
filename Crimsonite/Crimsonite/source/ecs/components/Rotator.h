#pragma once

#include "../ECS.h"

class Rotator : public EcsComponent
{

public:

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:

	glm::vec3 m_axis;
};