#include "MeshRenderer.h"

#include "../../mesh/Mesh.h"
#include "../../render/Renderer.h"
#include "../../render/Material.h"

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
	ImGui::NewLine();
	ImGui::Text("Name : '%s'", m_material->GetName().c_str());

	ImGui::Text("Shader : '%s'", m_material->GetShader()->GetName().c_str());

	float col [4];
	col[0] = m_material->GetColor().r;
	col[1] = m_material->GetColor().g;
	col[2] = m_material->GetColor().b;
	col[3] = m_material->GetColor().a;
	ImGui::Text("Main Color");
	ImGui::ColorEdit4("", col);
	m_material->SetColor(glm::vec4{ col[0], col[1], col[2], col[3] });
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
	glm::mat4 p = glm::perspective(glm::radians(60.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	glm::mat4 t = glm::translate(p, entity->transform.position);
	glm::mat4 y = glm::rotate(t, entity->transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 x = glm::rotate(y, entity->transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 z = glm::rotate(x, entity->transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	m_material->GetShader()->SetMvpMatrix(z);
}
