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
	virtual void DrawEditorProperties() override;

	glm::vec3 GetAxis() { return m_axis; }
	void SetAxis(glm::vec3 _axis) { m_axis = _axis; }

private:

	glm::vec3 m_axis;
};