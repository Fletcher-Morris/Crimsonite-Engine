#pragma once

#include "../ECS.h"
#include "../../mesh/Mesh.h"

class MeshRenderer : public EcsComponent
{

private:

	Mesh * m_mesh;

public:

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;


	void SetMesh(Mesh * _newMesh);
	void SetMesh(std::string _meshName);
};