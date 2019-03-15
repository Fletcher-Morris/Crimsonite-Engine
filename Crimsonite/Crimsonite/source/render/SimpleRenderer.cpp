#include "SimpleRenderer.h"
#include "../mesh/Mesh.h"
#include "../ecs/components/MeshRenderer.h"
#include "Shader.h"

void SimpleRenderer::Init()
{
	SetClearColor(0.863f, 0.78f, 0.235f);

	glEnable(GL_MULTISAMPLE);
}

void SimpleRenderer::Submit(Mesh * _mesh)
{
	if (_mesh->IsInitialized() == false) _mesh->UploadToGpu();
	m_meshes.push_back(_mesh);
}

void SimpleRenderer::Submit(Mesh * _mesh, Shader * _shader)
{
	if (_mesh->IsInitialized()) _mesh->UploadToGpu();
	m_meshes.push_back(_mesh);
	m_shaders.push_back(_shader);
}

void SimpleRenderer::Submit(MeshRenderer * _meshRenderer)
{
	if (_meshRenderer->GetMesh()->IsInitialized() == false)
		_meshRenderer->GetMesh()->UploadToGpu();
	m_meshRenderers.push_back(_meshRenderer);
}

void SimpleRenderer::Proccess()
{
}

void SimpleRenderer::Flush()
{
	glClearColor(p_r, p_g, p_g, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < m_meshes.size(); i++)
	{
		glBindVertexArray(m_meshes[i]->GetVao());
		glDrawElements(GL_TRIANGLES, m_meshes[i]->IndexCount(), GL_UNSIGNED_INT, 0);
	}
	for (int i = 0; i < m_meshRenderers.size(); i++)
	{
		MeshRenderer * m = m_meshRenderers[i];
		m->GetShader()->Bind();
		m->SetShaderMvp();

		glBindVertexArray(m->GetMesh()->GetVao());
		glDrawElements(GL_TRIANGLES, m->GetMesh()->IndexCount(), GL_UNSIGNED_INT, 0);
	}

	m_meshes.clear();
	m_meshRenderers.clear();
}

void SimpleRenderer::SetClearColor(float _clearColor[3])
{
	p_r = _clearColor[0];
	p_g = _clearColor[1];
	p_b = _clearColor[2];
}

void SimpleRenderer::SetClearColor(float _r, float _g, float _b)
{
	p_r = _r;
	p_g = _g;
	p_b = _b;
}
