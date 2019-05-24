#pragma once

#include "Renderer.h"

class Camera;
class Mesh;
class MeshRenderer;
class Shader;

class SimpleRenderer : public Renderer
{

private:

	//	A vector of Camera references.
	std::vector<Camera*> m_cameras;
	//	A vector of Mesh references.
	std::vector<Mesh*> m_meshes;
	//	A vector of Shader references.
	std::vector<Shader*> m_shaders;
	//	A vector of MeshRenderer references.
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

	virtual std::string Serialize() override;
	virtual void Deserialize(std::vector<std::string> _serializedValues) override;

};