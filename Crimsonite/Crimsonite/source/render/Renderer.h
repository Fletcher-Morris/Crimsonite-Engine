#pragma once

#include "Shader.h"
#include "../mesh/Mesh.h"

class Renderer
{
	
private:



public:

	virtual void Submit();
	virtual void Submit(Mesh * _mesh);
	virtual void Submit(Mesh * _mesh, Shader * _shader);
	virtual void submit(MeshRenderer * _meshRenderer);

};