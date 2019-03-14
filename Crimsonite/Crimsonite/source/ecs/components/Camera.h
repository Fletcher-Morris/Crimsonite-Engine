#pragma once

#include "../ECS.h"

#include <glm/common.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "../../render/CameraSettings.h"

class Camera : public EcsComponent
{

public:

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	void SetCameraSettings(CameraSettings _newSettings);
	void SetCameraSettings(float _fov);
	void SetCameraSettings(float _near, float _far);
	void SetCameraSettings(int _width, int _height);
	CameraSettings GetCameraSettings() { return m_settings; }

	void ReInit();
	const glm::mat4 GetProjectionMatrix() { return m_projectionMatrix; }
	const glm::mat4 GetViewMatrix() { return m_viewMatrix; }
	const glm::mat4 GetProjectionViewMatrix() { return m_projectionViewMatrix; }


private:

	CameraSettings m_settings;

	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionViewMatrix;

};