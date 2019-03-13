#pragma once

#include "Renderer.h"

class Mesh;
class MeshRenderer;
class Shader;

class SimpleRenderer : public Renderer
{

private:

	std::vector<Mesh*> m_meshes;
	std::vector<Shader*> m_shaders;
	std::vector<MeshRenderer*> m_meshRenderers;

public:

	virtual void Init() override;

	//virtual void Submit(Mesh * _mesh) override;
	//virtual void Submit(Mesh * _mesh, Shader * _shader) override;
	virtual void Submit(MeshRenderer * _meshRenderer) override;

	virtual void Proccess() override;
	virtual void Flush() override;

};