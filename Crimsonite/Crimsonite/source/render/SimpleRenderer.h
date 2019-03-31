#pragma once

#include "Renderer.h"

class Camera;
class Mesh;
class MeshRenderer;
class Shader;

class SimpleRenderer : public Renderer
{

private:

	std::vector<Camera*> m_cameras;
	std::vector<Mesh*> m_meshes;
	std::vector<Shader*> m_shaders;
	std::vector<MeshRenderer*> m_meshRenderers;

public:

	SimpleRenderer() { Init(); }

	virtual void Init() override;

	virtual void Submit(Camera * _cam) override;
	virtual void Submit(Mesh * _mesh) override;
	virtual void Submit(Mesh * _mesh, Shader * _shader) override;
	virtual void Submit(MeshRenderer * _meshRenderer) override;

	virtual void Proccess() override;
	virtual void Flush() override;

	virtual void SetClearColor(float _clearColor[3]) override;
	virtual void SetClearColor(float _r, float _g, float _b) override;

};