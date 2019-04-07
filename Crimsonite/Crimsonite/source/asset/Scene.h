#pragma once

#include <string>

#include "../ecs/ECS.h"
#include "../ecs/Components.h"
#include "../asset/AssetManager.h"
#include "../render/SimpleRenderer.h"


class Scene
{

public:

	Scene(std::string _scenePath);

	void Reload(std::string _scenePath);
	void Reload();

	void Deserialize();

	void Serialize();
	std::string GetSerializedString();
	void Save(std::string _scenePath);
	void Save();

	std::string GetName() { return m_name; }
	void SetName(std::string _sceneName) { m_name = _sceneName; }
	std::string GetPath() { return m_path; }
	void SetPath(std::string _path) { m_path = _path; }


	EcsSystem * ECS() { return m_ecs; }
	Renderer * Renderer() { return m_renderer; }

	void Update();
	void FixedUpdate();
	void Render();

	void FrameSizeChanged(int _width, int _height);

private:

	std::string m_serializedString;


	std::string m_name;
	std::string m_path;


	EcsSystem * m_ecs;
	SimpleRenderer * m_renderer;

};