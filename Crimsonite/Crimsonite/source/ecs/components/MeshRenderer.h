#pragma once

#include "../ECS.h"
#include "../../mesh/Mesh.h"
#include "../../render/SimpleRenderer.h"

class MeshRenderer : public EcsComponent
{

private:

	Mesh * m_mesh;
	Shader * m_shader;
	//Renderer * m_renderer;

public:

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;


	void SetMesh(Mesh * _newMesh);
	void SetMesh(std::string _meshName);
	Mesh * GetMesh() { return m_mesh; }

	/*void SetShader(Shader * _newShader);
	void SetShader(std::string _shaderName);
	Shader * GetShader() { return m_shader; }*/

	/*void SetRenderer(Renderer * _renderer);
	void SubmitToRenderer();
	void SubmitToRenderer(Renderer * _renderer);*/
};