#include "MeshRenderer.h"

#include "../../mesh/Mesh.h"
#include "../../render/Renderer.h"
#include "../../render/Material.h"
#include "Camera.h"
#include "../../render/MatrixMaths.h"

void MeshRenderer::OnInit()
{
}

void MeshRenderer::OnUpdate()
{
}

void MeshRenderer::OnFixedUpdate()
{
}

void MeshRenderer::OnRender()
{
	if (m_mesh == NULL) return;
	if (m_material == NULL) SetMaterial("default");
	SubmitToRenderer();
}

void MeshRenderer::OnEnable()
{
}

void MeshRenderer::OnDisable()
{
}

void MeshRenderer::DrawEditorProperties()
{
	std::string currentMeshName = m_mesh->GetName();
	if (currentMeshName == "" || m_mesh == NULL)
	{
		currentMeshName == "error";
	}
	if (ImGui::BeginCombo("Mesh", currentMeshName.c_str()))
	{
		for (int i = 0; i < AssetManager::Instance()->MeshCount(); i++)
		{
			std::string foundMeshName = AssetManager::Instance()->GetMesh(i)->GetName();
			bool isSelected = (currentMeshName == foundMeshName);
			if (ImGui::Button(foundMeshName.c_str()))
			{
				SetMesh(foundMeshName);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::NewLine();
	m_material->DrawEditorProperties();
}

std::string MeshRenderer::Serialize()
{
	std::string serialized = "val " + m_mesh->GetName();
	serialized += "\nval " + m_material->GetName();
	serialized += "\nval " + std::to_string(m_renderMode);
	return serialized;
}

void MeshRenderer::Deserialize(std::vector<std::string> _data)
{
	SetMesh(_data[1]);
	SetMaterial(_data[2]);
	SetRenderMode(std::stoi(_data[3]));
}

void MeshRenderer::SetMesh(Mesh * _newMesh)
{
	m_mesh = _newMesh;
}

void MeshRenderer::SetMesh(std::string _meshName)
{
	SetMesh(AssetManager::Instance()->GetMesh(_meshName));
}

void MeshRenderer::SetMaterial(Material * _newMaterial)
{
	m_material = _newMaterial;
}

void MeshRenderer::SetMaterial(std::string _materialName)
{
	SetMaterial(AssetManager::Instance()->GetMaterial(_materialName));
}

Material * MeshRenderer::GetMaterial()
{
	return m_material;
}

void MeshRenderer::SetRenderer(Renderer * _renderer)
{
	if (_renderer != NULL)
	{
		m_renderer = _renderer;
		if (m_mesh != NULL)
		{
			m_renderer->Submit(this);
		}
	}
	else
	std::cout << "NO RENDERER" << std::endl;
}

void MeshRenderer::SubmitToRenderer()
{
	if (m_mesh == NULL) return;
	if (m_renderer != NULL)
	{
		m_renderer->Submit(this);
	}
	else
	std::cout << "NO RENDERER" << std::endl;
}

void MeshRenderer::SubmitToRenderer(Renderer * _renderer)
{
	if (_renderer != NULL)
	{
		m_renderer = _renderer;
		SubmitToRenderer();
	}
	else
	std::cout << "NO RENDERER" << std::endl;
}

void MeshRenderer::SetShaderMvp()
{
	m_projMatrix = CreateProjectionMatrix(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	UpdateShaderMvp();
}

void MeshRenderer::SetShaderMvp(Camera * _camera)
{
	m_viewMatrix = _camera->GetViewMatrix();
	m_projMatrix = _camera->GetProjectionMatrix();
	UpdateShaderMvp();
}

void MeshRenderer::UpdateShaderMvp()
{
	m_modelMatrix = CreateModelMatrix(entity->transform);
	m_mvpMatrix = m_projMatrix * m_viewMatrix *  m_modelMatrix;

	m_material->GetShader()->SetMvpMatrix(m_modelMatrix, m_viewMatrix, m_projMatrix);
}
