#pragma once

#include <vector>

class Camera;
class Mesh;
class MeshRenderer;
class Shader;

class Renderer
{
	
protected:

	float p_r = 0.0f;
	float p_g = 0.0f;
	float p_b = 0.0f;

public:

	virtual void Init() {}

	virtual void Submit(Camera * _cam) {}
	virtual void Submit(Mesh * _mesh) {}
	virtual void Submit(Mesh * _mesh, Shader * _shader) {}
	virtual void Submit(MeshRenderer * _meshRenderer) {}

	virtual void Proccess() {}
	virtual void Flush() {}

	virtual void SetClearColor(float _clearColor [3]) {}
	virtual void SetClearColor(float _r, float _g, float _b) {}

	virtual std::string Serialize() { return  ""; }
	virtual void Deserlialize(std::vector<std::string> _serializedValues) {}

};