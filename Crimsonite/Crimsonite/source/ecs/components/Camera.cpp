#include "Camera.h"

void Camera::SetCameraSettings(CameraSettings _newSettings)
{
	m_settings = _newSettings;
}

void Camera::SetCameraSettings(float _fov)
{
	CameraSettings newSettings;
	newSettings.fov = _fov;
	SetCameraSettings(newSettings);
}

void Camera::SetCameraSettings(float _near, float _far)
{
	CameraSettings newSettings;
	newSettings.nearClip = _near;
	newSettings.farClip = _far;
	SetCameraSettings(newSettings);
}

void Camera::SetCameraSettings(int _width, int _height)
{
	CameraSettings newSettings;
	newSettings.width = _width;
	newSettings.height = _height;
	SetCameraSettings(newSettings);
}

void Camera::ReInit()
{

}
