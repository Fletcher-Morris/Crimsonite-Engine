#include "Scene.h"
#include <iostream>
#include <fstream>
#include <vector>

#include "../core/Window.h"
#include "../ecs/ECS.h"
#include "../ecs/Components.h"
#include "../asset/AssetManager.h"
#include "../render/SimpleRenderer.h"

#define DESCRIBING_NULL 0
#define DESCRIBING_ENTITY 1
#define DESCRIBING_COMPONENT 2
#define DESCRIBING_RENDERER 3

Scene::Scene()
{
	m_name = "New Scene";
	m_path = "";
	m_serializedString = "";

	m_renderer = new SimpleRenderer();
	m_ecs = new EcsSystem();
}

Scene::Scene(std::string _scenePath)
{
	m_path = _scenePath;
	m_renderer = new SimpleRenderer();
	Reload(_scenePath);
}

void Scene::Reload(std::string _scenePath)
{

	m_ecs = new EcsSystem();

#pragma warning(push)
#pragma warning(disable: 4996)
	FILE * file = fopen((_scenePath + ".crimsn").c_str(), "r");
	int res = 0;
	int descType = 0;
	int prevType = 0;
	std::vector<std::string> descriptionLines;
	while (res != EOF)
	{
		char lineData[256];
		res = fscanf(file, "%s", lineData);

		if (res == EOF) break;

		prevType = descType;

		if (strcmp(lineData, "SceneName") == 0)
		{
			fscanf(file, "%s", &lineData);
			SetName((std::string)lineData);
		}
		else if (strcmp(lineData, "BeginEntity") == 0)
		{
			descType = DESCRIBING_ENTITY;
		}
		else if (strcmp(lineData, "EndEntity") == 0)
		{
			descType = DESCRIBING_NULL;
			m_ecs->DeserializeEntity(descriptionLines);
			descriptionLines.clear();
		}
		else if (strcmp(lineData, "BeginComponent") == 0)
		{
			descType = DESCRIBING_COMPONENT;
			fscanf(file, "%s", &lineData);
			descriptionLines.push_back((std::string)lineData);
		}
		else if (strcmp(lineData, "EndComponent") == 0)
		{
			descType = DESCRIBING_NULL;
			m_ecs->DeserializeComponent(descriptionLines);
			std::string componentName = descriptionLines[0];
			if (componentName == "Camera")
			{
				Camera * comp = &m_ecs->NewestEntity()->AttachComponent<Camera>();
				comp->SetRenderer(m_renderer);
				if (descriptionLines.size() >= 1)
				comp->Deserialize(descriptionLines);
			}
			else if (componentName == "MeshRenderer")
			{
				MeshRenderer * comp = &m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
				comp->SetRenderer(m_renderer);
				if (descriptionLines.size() >= 1)
				comp->Deserialize(descriptionLines);
			}
			else if (componentName == "Rotator")
			{
				Rotator * comp = &m_ecs->NewestEntity()->AttachComponent<Rotator>();
				if(descriptionLines.size() >= 1)
				comp->Deserialize(descriptionLines);
			}
			descriptionLines.clear();
		}
		else if (strcmp(lineData, "BeginRenderer") == 0)
		{
			descType = DESCRIBING_RENDERER;
		}
		else if (strcmp(lineData, "EndRenderer") == 0)
		{
			descType = DESCRIBING_NULL;
			m_renderer->Deserialize(descriptionLines);
			descriptionLines.clear();
		}
		else if (strcmp(lineData, "val") == 0)
		{
			fscanf(file, "%s", &lineData);
			descriptionLines.push_back((std::string)lineData);
		}
	}
#pragma warning(pop)
}

void Scene::Reload()
{
	Reload(m_path);
}

void Scene::Serialize()
{
	m_serializedString = "SceneName " + GetName();
	m_serializedString += "\n\n";
	m_serializedString += m_renderer->Serialize();
	m_serializedString += "\n\n";
	m_serializedString += m_ecs->Serialize();
}

std::string Scene::GetSerializedString()
{
	return m_serializedString;
}

void Scene::Save(std::string _scenePath)
{
	m_path = _scenePath;
	Serialize();
	std::ofstream file(m_path + ".crimsn");
	file << "# Crimsonite Scene File\n" << std::endl;
	file << GetSerializedString();
	file.close();
	std::cout << "Saved scene '" << GetName() << "' to '" << m_path << "'." << std::endl;
}

void Scene::Save()
{
	Save(m_path);
}

void Scene::Update()
{
	for (auto& ent : m_ecs->entities)ent->Update();
}

void Scene::FixedUpdate()
{
	for (auto& ent : m_ecs->entities)ent->FixedUpdate();
}

void Scene::Render()
{
	for (auto& ent : m_ecs->entities)ent->Render();
	m_renderer->Proccess();
	m_renderer->Flush();
}

void Scene::FrameSizeChanged(int _width, int _height)
{
}
