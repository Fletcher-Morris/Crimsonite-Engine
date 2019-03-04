#pragma once

#include "Shader.h"
#include "../ecs/components/MeshRenderer.h"

class MeshRenderer;

class Renderer
{
	
private:



public:

	virtual void Init();

	virtual void Submit();
	virtual void Submit(Mesh * _mesh);
	virtual void Submit(Mesh * _mesh, Shader * _shader);
	virtual void submit(MeshRenderer * _meshRenderer);

};