#include "MeshRenderer.h"

#include "../../mesh/Mesh.h"
#include "../../render/SimpleRenderer.h"
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
	std::string currentMeshName = GetMesh()->GetName();
	if (ImGui::BeginCombo("Mesh", currentMeshName.c_str()))
	{
		for (int i = 0; i < AssetManager::MeshCount(); i++)
		{
			std::string foundMeshName = AssetManager::GetMesh(i)->GetName();
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
	std::string currentMaterialName = GetMaterial()->GetName();
	if (ImGui::BeginCombo("Material", currentMaterialName.c_str()))
	{
		for (int i = 0; i < AssetManager::MaterialCount(); i++)
		{
			std::string foundMaterialName = AssetManager::GetMaterial(i)->GetName();
			bool isSelected = (currentMaterialName == foundMaterialName);
			if (ImGui::Button(foundMaterialName.c_str()))
			{
				SetMaterial(foundMaterialName);
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

	DrawAdditionalWireframe(true);
}

std::string MeshRenderer::Serialize()
{
	std::string serialized = "";
	if (!IsDetached())
	{
		serialized += "val " + m_mesh->GetName();
		serialized += "\nval " + m_material->GetName();
		serialized += "\nval " + std::to_string(m_renderMode);
	}
	return serialized;
}

void MeshRenderer::Deserialize(std::vector<std::string> _data)
{
	if (_data.size() != 5) return;
	//	_data[0] is just the component name.
	//	_data[1] is just the enabled state.
	//	_data[2] is name of the mesh to use.
	//	_data[3] is name of the material to use.
	//	_data[4] is render mode to use.
	SetMesh(_data[2]);
	SetMaterial(_data[3]);
	SetRenderMode(std::stoi(_data[4]));
}

void MeshRenderer::SetMesh(Mesh * _newMesh)
{
	m_mesh = _newMesh;
}

void MeshRenderer::SetMesh(std::string _meshName)
{
	SetMesh(AssetManager::GetMesh(_meshName));
}

Mesh * MeshRenderer::GetMesh()
{
	if (m_mesh == NULL) SetMesh("error");
	return m_mesh;
}

void MeshRenderer::SetMaterial(Material * _newMaterial)
{
	m_material = _newMaterial;
}

void MeshRenderer::SetMaterial(std::string _materialName)
{
	SetMaterial(AssetManager::GetMaterial(_materialName));
}

Material * MeshRenderer::GetMaterial()
{
	if (m_material == NULL) SetMaterial("default");
	return m_material;
}

void MeshRenderer::SetRenderer(Renderer * _renderer)
{
	if (_renderer != NULL)
	{
		m_renderer = _renderer;
	}
}

void MeshRenderer::SubmitToRenderer()
{
	if (m_mesh == NULL) return;
	if (m_renderer == NULL)
	{
		SetRenderer(Scene::Current()->Renderer());
	}
	m_renderer->Submit(this);
}

void MeshRenderer::SubmitToRenderer(Renderer * _renderer)
{
	if (_renderer != NULL)
	{
		m_renderer = _renderer;
	}
	SubmitToRenderer();
}

void MeshRenderer::DrawAdditionalWireframe(bool _draw)
{
	m_drawWireframe = _draw;
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
	m_modelMatrix = CreateModelMatrix(&entity->transform);
	m_mvpMatrix = m_projMatrix * m_viewMatrix *  m_modelMatrix;
	m_material->GetShader()->SetMvpMatrix(m_modelMatrix, m_viewMatrix, m_projMatrix);
}
