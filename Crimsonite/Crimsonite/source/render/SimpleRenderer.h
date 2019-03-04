#pragma once

#include "Renderer.h"

class SimpleRenderer : public Renderer
{

private:

	std::vector<MeshRenderer*> m_meshRenderers;

public:

	virtual void Init() override;

	virtual void Submit(Mesh * _mesh) override;
	virtual void Submit(Mesh * _mesh, Shader * _shader) override;
	virtual void submit(MeshRenderer * _meshRenderer) override;

	virtual void Proccess() override;
	virtual void Flush() override;

};