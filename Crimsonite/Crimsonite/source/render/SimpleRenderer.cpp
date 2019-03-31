#include "SimpleRenderer.h"
#include "../mesh/Mesh.h"
#include "../ecs/components/MeshRenderer.h"
#include "../ecs/components/Camera.h"
#include "Shader.h"

void SimpleRenderer::Init()
{
	SetClearColor(0.2f, 0.2f, 0.2f);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void SimpleRenderer::Submit(Camera * _cam)
{
	m_cameras.push_back(_cam);
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

	for (int c = 0; c < m_cameras.size(); c++)
	{
		m_cameras[c]->Bind();

		glClearColor(p_r, p_g, p_g, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);


		if (!m_cameras[c]->GetOutputFrameBuffer())
		{
			glViewport(0, 0, m_cameras[c]->GetOutputFrameBuffer()->GetLinkedTexture()->GetWidth(), m_cameras[c]->GetOutputFrameBuffer()->GetLinkedTexture()->GetHeight());
		}

		for (int i = 0; i < m_meshes.size(); i++)
		{
			glBindVertexArray(m_meshes[i]->GetVao());
			glDrawElements(GL_TRIANGLES, m_meshes[i]->IndexCount(), GL_UNSIGNED_INT, 0);
		}
		for (int i = 0; i < m_meshRenderers.size(); i++)
		{
			MeshRenderer * m = m_meshRenderers[i];
			m->GetMaterial()->GetShader()->Bind();
			m->GetMaterial()->UpdateShaderProperties();
			m->SetShaderMvp();

			glBindVertexArray(m->GetMesh()->GetVao());
			glDrawElements(m->GetRenderMode(), m->GetMesh()->IndexCount(), GL_UNSIGNED_INT, 0);
		}

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	AssetManager::Instance()->GetPassthroughShader()->Bind();
	glBindVertexArray(AssetManager::Instance()->GetMesh("quad")->GetVao());
	glDisable(GL_DEPTH_TEST);
	AssetManager::Instance()->GetTexture("viewport")->Bind();
	glDrawElements(GL_TRIANGLES, AssetManager::Instance()->GetMesh("quad")->IndexCount(), GL_UNSIGNED_INT, 0);

	m_cameras.clear();
	m_meshes.clear();
	m_meshRenderers.clear();
	glBindTexture(GL_TEXTURE_2D, 0);
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
