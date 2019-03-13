#include "MeshRenderer.h"

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
}

void MeshRenderer::SetMesh(std::string _meshName)
{
	m_mesh = AssetManager::Instance()->GetMesh(_meshName);
}

//void MeshRenderer::SetShader(Shader * _newShader)
//{
//	m_shader = _newShader;
//}
//
//void MeshRenderer::SetShader(std::string _shaderName)
//{
//	m_shader = AssetManager::Instance()->GetShader(_shaderName);
//}

//void MeshRenderer::SetRenderer(Renderer * _renderer)
//{
//	if (_renderer != NULL)
//	{
//		m_renderer = _renderer;
//		m_renderer->Submit(this);
//	}
//}
//
//void MeshRenderer::SubmitToRenderer()
//{
//	if (m_renderer != NULL)
//	{
//		m_renderer->Submit(this);
//	}
//}
//
//void MeshRenderer::SubmitToRenderer(Renderer * _renderer)
//{
//	if (_renderer != NULL)
//	{
//		m_renderer = _renderer;
//		m_renderer->Submit(this);
//	}
//}
