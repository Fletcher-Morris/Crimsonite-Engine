#include "SimpleRenderer.h"

void SimpleRenderer::Init()
{
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

//void SimpleRenderer::Submit(MeshRenderer * _meshRenderer)
//{
//	if (_meshRenderer->GetMesh()->IsInitialized() == false)
//		_meshRenderer->GetMesh()->UploadToGpu();
//	m_meshRenderers.push_back(_meshRenderer);
//}

void SimpleRenderer::Proccess()
{
}

void SimpleRenderer::Flush()
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		glBindVertexArray(m_meshes[i]->GetVao());
		glDrawElements(GL_TRIANGLES, m_meshes[i]->IndexCount(), GL_UNSIGNED_INT, 0);
	}
	/*for (int i = 0; i < m_meshRenderers.size(); i++)
	{
		glBindVertexArray(m_meshRenderers[i]->GetMesh()->GetVao());
		glDrawElements(GL_TRIANGLES, m_meshRenderers[i]->GetMesh()->IndexCount(), GL_UNSIGNED_INT, 0);
	}*/
}
