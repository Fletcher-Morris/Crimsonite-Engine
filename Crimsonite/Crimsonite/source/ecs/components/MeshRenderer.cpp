#include "MeshRenderer.h"

#include "../../mesh/Mesh.h"
#include "../../render/Shader.h"
#include "../../render/Renderer.h"

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
		m_renderer->Submit(this);
	}
	else
	std::cout << "NO RENDERER" << std::endl;
}

void MeshRenderer::SubmitToRenderer()
{
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
		m_renderer->Submit(this);
	}
	else
	std::cout << "NO RENDERER" << std::endl;
}
