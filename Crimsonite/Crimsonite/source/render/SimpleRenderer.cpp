#include "SimpleRenderer.h"

void SimpleRenderer::Init()
{
}

void SimpleRenderer::Submit(Mesh * _mesh)
{
	if (_mesh->IsInitialized() == false) _mesh->UploadToGpu();
}

void SimpleRenderer::Submit(Mesh * _mesh, Shader * _shader)
{
	if (_mesh->IsInitialized()) _mesh->UploadToGpu();
}

void SimpleRenderer::submit(MeshRenderer * _meshRenderer)
{
	if (_meshRenderer->GetMesh()->IsInitialized() == false)
		_meshRenderer->GetMesh()->UploadToGpu();
}

void SimpleRenderer::Proccess()
{
}

void SimpleRenderer::Flush()
{
}
