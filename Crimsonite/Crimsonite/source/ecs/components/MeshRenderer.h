#pragma once

#include "../ECS.h"

class Mesh;
class Material;
class Renderer;

class MeshRenderer : public EcsComponent
{

private:

	Mesh * m_mesh;
	Material * m_material;
	Renderer * m_renderer;

	int m_renderMode = GL_TRIANGLES;

	int i = 0;

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

	void SetMaterial(Material * _newMaterial);
	void SetMaterial(std::string _materialName);
	Material * GetMaterial();

	void SetRenderer(Renderer * _renderer);
	void SubmitToRenderer();
	void SubmitToRenderer(Renderer * _renderer);

	void SetShaderMvp();

	void SetRenderMode(int _mode)
	{
		m_renderMode = _mode;
		std::cout << "Set Mesh Render Mode Of '" << entity->GetName() << "' To '" << m_renderMode << "'." << std::endl;
	}
	int GetRenderMode() { return m_renderMode; }
};