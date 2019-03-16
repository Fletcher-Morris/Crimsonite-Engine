#include "MeshRenderer.h"

#include "../../mesh/Mesh.h"
#include "../../render/Shader.h"
#include "../../render/Renderer.h"

void MeshRenderer::OnInit()
{
}

void MeshRenderer::OnUpdate()
{
	entity->transform.Rotate(0.0f, 0.002f, 0.0f);
}

void MeshRenderer::OnFixedUpdate()
{
}

void MeshRenderer::OnRender()
{
	SubmitToRenderer();
}

void MeshRenderer::OnEnable()
{
}

void MeshRenderer::OnDisable()
{
}

void MeshRenderer::SetMesh(Mesh * _newMesh)
{
	m_mesh = _newMesh;
	std::cout << entity->GetName() << " : MeshRenderer : SetMesh() : " << m_mesh->IndexCount() << std::endl;
}

void MeshRenderer::SetMesh(std::string _meshName)
{
	SetMesh(AssetManager::Instance()->GetMesh(_meshName));
}

void MeshRenderer::SetShader(Shader * _newShader)
{
	m_shader = _newShader;
}

void MeshRenderer::SetShader(std::string _shaderName)
{
	m_shader = AssetManager::Instance()->GetShader(_shaderName);
}

Shader * MeshRenderer::GetShader()
{
	return m_shader;
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

	m_shader->SetMvpMatrix(z);
}
