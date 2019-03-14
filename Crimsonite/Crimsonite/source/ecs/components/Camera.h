#pragma once

#include "../ECS.h"
#include <glm/common.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


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

private:

	CameraSettings m_settings;

};

struct CameraSettings
{
	int width = 265;
	int height = 256;
	float fov = 60.0f;
	float nearClip = 0.1f;
	float farClip = 100.0f;
};