#include "Camera.h"

#include "../../render/MatrixMaths.h"

#include "../../render/Renderer.h"

void Camera::OnInit()
{
	ReInit();
}

void Camera::OnUpdate()
{
}

void Camera::OnFixedUpdate()
{
}

void Camera::OnRender()
{
	if (!m_renderer) return;
	m_renderer->Submit(this);
}

void Camera::OnEnable()
{
}

void Camera::OnDisable()
{
}

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
	std::cout << _width << ", " << _height << std::endl;
	SetCameraSettings(newSettings);
}

void Camera::Bind()
{
	if (m_frameBuffer == 0)	{}
	else
	{
		m_frameBuffer->Bind();
	}
}

void Camera::ReInit()
{
	m_projectionMatrix = CreateProjectionMatrix(m_settings);
	m_viewMatrix = CreateViewMatrix(*this);
	m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
}
