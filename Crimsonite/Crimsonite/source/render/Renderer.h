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
	int p_renderModeOverride = -1;

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
	virtual float * GetClearColor() { return new float[3]{ p_r,p_g,p_b }; }

	void SetOverrideMode(int _renderMode) { p_renderModeOverride = _renderMode; }
	int GetOverrideMode() { return p_renderModeOverride; }

	virtual std::string Serialize() { return  ""; }
	virtual void Deserialize(std::vector<std::string> _serializedValues) {}

};