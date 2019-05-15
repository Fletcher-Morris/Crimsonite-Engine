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
	if (dirty) SetCameraClipping(Window::Width(), Window::Height());

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
	CameraSettings newSettings = GetCameraSettings();
	ImGui::InputFloat("FOV", &newSettings.fov);
	ImGui::InputFloat("Near Clip", &newSettings.nearClip);
	ImGui::InputFloat("Far Clip", &newSettings.farClip);
	SetCameraSettings(newSettings);
	ImGui::Text("Width : %i", m_settings.width);
	ImGui::Text("Height : %i", m_settings.height);
	ImGui::Text("Framebuffer : %s", GetFrameBufferName().c_str());
	if (m_frameBuffer != NULL)
	{
		if (ImGui::Button("Detach FrameBuffer"))
		{
			m_frameBuffer = NULL;
		}
	}
	else
	{
		if (ImGui::Button("Attach FrameBuffer"))
		{
			EnforceFrameBuffer();
		}
	}
}

std::string Camera::Serialize()
{
	std::string serialized = "";
	if (!IsDetached())
	{
		serialized += "val " + std::to_string(m_settings.fov);
		serialized += "\nval " + std::to_string(m_settings.nearClip);
		serialized += "\nval " + std::to_string(m_settings.farClip);
		serialized += "\nval " + GetFrameBufferName();
	}
	return serialized;
}

void Camera::Deserialize(std::vector<std::string> _data)
{
	if (_data.size() != 6) return;
	//	_data[0] is just the component name.
	//	_data[1] is just the enabled state.
	//	_data[2] is the field of view.
	//	_data[3] is the near clipping plane.
	//	_data[4] is the far clipping plane.
	//	_data[5] is the name of the FrameBuffer.
	SetCameraSettings(std::stof(_data[2]), std::stof(_data[3]), std::stof(_data[4]));
	if (_data[5] != "NO_BUFFER")
	{
		if (_data[5] != entity->GetName() + "_FrameBuffer")
		{
			EnforceFrameBuffer();
		}
		else
		{
			SetOutputFrameBuffer(_data[5]);
		}
	}
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

void Camera::SetCameraFov(float _fov)
{
	CameraSettings newSettings = GetCameraSettings();
	newSettings.fov = _fov;
	SetCameraSettings(newSettings);
}

void Camera::SetCameraClipping(float _near, float _far)
{
	//CameraSettings newSettings = GetCameraSettings();
	//newSettings.nearClip = _near;
	//newSettings.farClip = _far;
	//SetCameraSettings(newSettings);
}

void Camera::SetCameraSettings(float _fov, float _near, float _far)
{
	//CameraSettings newSettings = GetCameraSettings();
	//newSettings.fov = _fov;
	//newSettings.nearClip = _near;
	//newSettings.farClip = _far;
	//SetCameraSettings(newSettings);
}

void Camera::SetCameraSize(int _width, int _height)
{
	CameraSettings newSettings = GetCameraSettings();
	newSettings.width = _width;
	newSettings.height = _height;
	SetCameraSettings(newSettings);
	ResizeFramebuffer(_width, _height);
}

void Camera::Bind()
{
	if(m_frameBuffer)
	m_frameBuffer->Bind();
}

void Camera::ReInit()
{
	m_projectionMatrix = CreateProjectionMatrix(m_settings);
	EnforceFrameBuffer();
}

void Camera::CreateCameraViewMatrix()
{
	m_viewMatrix = CreateViewMatrix(*this);
}

void Camera::ResizeFramebuffer(int _width, int _height)
{
	if (GetOutputFrameBuffer() != NULL)
	{
		GetOutputFrameBuffer()->Resize(_width, _height);
	}
}

void Camera::EnforceFrameBuffer()
{
	if (m_frameBuffer == NULL)
	{
		SetOutputFrameBuffer(entity->GetName() + "_FrameBuffer", true);
	}
}
