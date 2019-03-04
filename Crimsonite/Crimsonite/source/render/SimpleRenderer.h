#pragma once

#include "Renderer.h"

class SimpleRenderer : public Renderer
{

public:

	virtual void Init() override;

	virtual void Submit() override;
	virtual void Submit(Mesh * _mesh) override;
	virtual void Submit(Mesh * _mesh, Shader * _shader) override;
	virtual void submit(MeshRenderer * _meshRenderer) override;

};