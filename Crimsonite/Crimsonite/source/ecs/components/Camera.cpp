#include "Camera.h"

#include "../../render/MatrixMaths.h"
#include "../../render/Renderer.h"
#include "../../core/CrimsonCore.h"

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

	bool dirty = false;
	if (m_settings.width != Window::Width()) dirty = true;
	if (m_settings.height != Window::Height()) dirty = true;
	if (dirty) SetCameraSettings(Window::Width(), Window::Height());

	m_renderer->Submit(this);
}

void Camera::OnEnable()
{
}

void Camera::OnDisable()
{
}

void Camera::DrawEditorProperties()
{
	if (m_frameBuffer)
	{
		ImGui::Image((GLuint*)m_frameBuffer->GetLinkedTexture()->TextureId, ImVec2(267, 150));
	}
	float newFov = GetCameraSettings().fov;
	ImGui::DragFloat("FOV", &newFov, 1.0f, 179.0f);
	SetCameraSettings(newFov);
	ImGui::Text("Width : %i", m_settings.width);
	ImGui::Text("Height : %i", m_settings.height);
	//ImGui::DragFloat("Near Clip", &newSettings.nearClip, 0.01f, 1000.0f);
	//ImGui::DragFloat("Far Clip", &newSettings.farClip, 0.01f, 1000.0f);

}

std::string Camera::Serialize()
{
	std::string serialized = "val " + std::to_string(m_settings.fov);
	serialized += "\nval " + std::to_string(m_settings.nearClip);
	serialized += "\nval " + std::to_string(m_settings.farClip);
	serialized += "\nval " + m_frameBuffer->GetName();
	return serialized;
}

void Camera::Deserialize(std::vector<std::string> _data)
{
	//	_data[0] is just the component name.
	//	_data[1] is the field of view.
	//	_data[2] is the near clipping plane.
	//	_data[3] is the far clipping plane.
	//	_data[4] is the name of the FrameBuffer.
	SetCameraSettings(std::stof(_data[1]), std::stof(_data[2]), std::stof(_data[3]));
	SetOutputFrameBuffer(_data[4]);
}

void Camera::SetCameraSettings(CameraSettings _newSettings)
{
	if
	(
	_newSettings.fov != m_settings.fov ||
	_newSettings.width != m_settings.width ||
	_newSettings.height != m_settings.height ||
	_newSettings.nearClip != m_settings.nearClip ||
	_newSettings.farClip != m_settings.farClip
	)
	{
		m_settings = _newSettings;
		ReInit();
	}
}

void Camera::SetCameraSettings(float _fov)
{
	CameraSettings newSettings = GetCameraSettings();
	newSettings.fov = _fov;
	SetCameraSettings(newSettings);
}

void Camera::SetCameraSettings(float _near, float _far)
{
	CameraSettings newSettings = GetCameraSettings();
	newSettings.nearClip = _near;
	newSettings.farClip = _far;
	SetCameraSettings(newSettings);
}

void Camera::SetCameraSettings(float _fov, float _near, float _far)
{
	CameraSettings newSettings = GetCameraSettings();
	newSettings.fov = _fov;
	newSettings.nearClip = _near;
	newSettings.farClip = _far;
	SetCameraSettings(newSettings);
}

void Camera::SetCameraSettings(int _width, int _height)
{
	CameraSettings newSettings = GetCameraSettings();
	newSettings.width = _width;
	newSettings.height = _height;
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
	std::cout << "Initialised Camera '" << entity->GetName() << "'." << std::endl;
}

void Camera::CreateCameraViewMatrix()
{
	m_viewMatrix = CreateViewMatrix(*this);
}

void Camera::UpdateOutputBufferSize()
{
}
