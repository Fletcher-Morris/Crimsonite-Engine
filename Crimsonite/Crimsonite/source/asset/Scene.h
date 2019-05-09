#pragma once

#include <string>
#include <vector>

class EcsSystem;
class Renderer;
class SimpleRenderer;

class Scene
{

public:

	Scene();
	Scene(std::string _scenePath);

	void LoadFromPath(std::string _scenePath);
	void Reload();

	void Deserialize();

	void Serialize();
	std::string GetSerializedString();
	void Save(std::string _scenePath);
	void Save();

	std::string GetName() { return m_name; }
	void SetName(std::string _sceneName);
	std::string GetPath() { return m_path; }
	void SetPath(std::string _path) { m_path = _path; }
	void SetSceneId(int _id) { m_sceneId = _id; }
	int GetSceneId() { return m_sceneId; }


	EcsSystem * ECS() { return m_ecs; }
	SimpleRenderer * Renderer() { return m_renderer; }

	static Scene * Current();

	void Update();
	void FixedUpdate();
	void Render();

	void FrameSizeChanged(int _width, int _height);

private:

	std::string m_serializedString;
	std::vector<std::string> m_initialVector;
	std::vector<std::string> m_serializedVector;


	std::string m_name;
	std::string m_path;
	int m_sceneId;


	EcsSystem * m_ecs;
	SimpleRenderer * m_renderer;

};