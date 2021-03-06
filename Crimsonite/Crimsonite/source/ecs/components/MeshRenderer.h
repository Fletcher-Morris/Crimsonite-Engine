#pragma once

#include "../ECS.h"

class Mesh;
class Material;
class Renderer;
class Camera;

class MeshRenderer : public EcsComponent
{

private:

	Mesh * m_mesh;
	Material * m_material;
	Renderer * m_renderer;

	int m_renderMode = GL_TRIANGLES;
	bool m_drawWireframe = false;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projMatrix;
	glm::mat4 m_mvpMatrix;

public:

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void DrawEditorProperties() override;
	virtual std::string Serialize() override;
	virtual void Deserialize(std::vector<std::string> _data) override;

	void SetMesh(Mesh * _newMesh);
	void SetMesh(std::string _meshName);
	Mesh * GetMesh();

	void SetMaterial(Material * _newMaterial);
	void SetMaterial(std::string _materialName);
	Material * GetMaterial();

	void SetRenderer(Renderer * _renderer);
	void SubmitToRenderer();
	void SubmitToRenderer(Renderer * _renderer);

	bool DoDrawWireframe() { return m_drawWireframe; }
	void DrawAdditionalWireframe(bool _draw);

	void SetShaderMvp();
	void SetShaderMvp(Camera * _camera);
	void SetShaderMvp(float _fov, float _ratio, float _near, float _far);
	void UpdateShaderMvp();

	glm::mat4 GetModelMatrix() { return m_modelMatrix; }
	glm::mat4 GetViewMatrix() { return m_viewMatrix; }
	glm::mat4 GetProjMatrix() { return m_projMatrix; }

	void SetRenderMode(int _mode)
	{
		m_renderMode = _mode;
	}
	int GetRenderMode() { return m_renderMode; }
};