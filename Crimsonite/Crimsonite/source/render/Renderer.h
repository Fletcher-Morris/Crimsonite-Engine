#pragma once

#include <vector>

//#include "Shader.h"
//#include "../ecs/components/MeshRenderer.h"
//#include "../mesh/Mesh.h"

class Mesh;
class MeshRenderer;
class Shader;

class Renderer
{
	
private:



public:

	virtual void Init() {}

	//virtual void Submit(Mesh * _mesh);
	//virtual void Submit(Mesh * _mesh, Shader * _shader);
	virtual void Submit(MeshRenderer * _meshRenderer) {}

	virtual void Proccess() {}
	virtual void Flush() {}

};