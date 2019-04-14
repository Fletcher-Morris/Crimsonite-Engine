#include "Scene.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../core/Window.h"
#include "../ecs/ECS.h"
#include "../ecs/Components.h"
#include "../asset/AssetManager.h"
#include "../render/SimpleRenderer.h"

Scene::Scene()
{
	m_name = "NewScene";
	m_path = "";
	m_serializedString = "";

	m_renderer = new SimpleRenderer();
	m_ecs = new EcsSystem();

	m_ecs->NewEntity("MainCamera");
	Camera * cam = &m_ecs->NewestEntity()->AttachComponent<Camera>();
	cam->SetOutputFrameBuffer("MainCamBuffer");
}

Scene::Scene(std::string _scenePath)
{
	m_path = _scenePath;
	m_renderer = new SimpleRenderer();
	LoadFromPath(_scenePath);
}

void Scene::LoadFromPath(std::string _scenePath)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	m_initialVector.clear();
	m_serializedVector.clear();
	std::ifstream fileStream((_scenePath + ".crimsn").c_str());
	std::string line;
	while (std::getline(fileStream, line))
	{
		if(line != "")
		m_initialVector.push_back(line);
	}
	Deserialize();
	std::cout << "Loaded scene '" << GetName() << "' from '" << m_path << "'." << std::endl;
#pragma warning(pop)
}

void Scene::Reload()
{
	LoadFromPath(m_path);
}

void Scene::Deserialize()
{
	m_ecs = new EcsSystem();
	m_serializedVector = m_initialVector;
	std::vector<std::string> descriptionLines;
	int res = 0;
#pragma warning(push)
#pragma warning(disable: 4996)
	for (int i = 0; i < m_serializedVector.size(); i++)
	{
		char lineData[512];
		const char * lineHeader = m_serializedVector[i].c_str();
		res = sscanf(lineHeader, "%s", &lineData);

		if (res != -1)
		{
			if (strcmp(lineData, "") == 0)
			{
			}
			else if (strcmp(lineData, "BeginScene") == 0)
			{
				descriptionLines.clear();
			}
			else if (strcmp(lineData, "EndScene") == 0)
			{
				SetName(descriptionLines[0]);
				descriptionLines.clear();
			}
			else if (strcmp(lineData, "BeginEntity") == 0)
			{
				descriptionLines.clear();
			}
			else if (strcmp(lineData, "EndEntity") == 0)
			{
				m_ecs->DeserializeEntity(descriptionLines);
				descriptionLines.clear();
			}
			else if (strcmp(lineData, "BeginComponent") == 0)
			{
				descriptionLines.clear();
				std::string value = m_serializedVector[i];
				value.erase(0, 15);
				descriptionLines.push_back(value);
			}
			else if (strcmp(lineData, "EndComponent") == 0)
			{
				m_ecs->DeserializeComponent(descriptionLines);
				std::string componentName = descriptionLines[0];
				if (componentName == "Camera")
				{
					Camera * comp = &m_ecs->NewestEntity()->AttachComponent<Camera>();
					comp->SetRenderer(m_renderer);
					if (descriptionLines.size() >= 1)
					{
						comp->SetEnabled(descriptionLines[1] == "true" ? true : false);
						comp->Deserialize(descriptionLines);
					}
				}
				else if (componentName == "MeshRenderer")
				{
					MeshRenderer * comp = &m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
					comp->SetRenderer(m_renderer);
					if (descriptionLines.size() >= 1)
					{
						comp->SetEnabled(descriptionLines[1] == "true" ? true : false);
						comp->Deserialize(descriptionLines);
					}
				}
				else if (componentName == "Rotator")
				{
					Rotator * comp = &m_ecs->NewestEntity()->AttachComponent<Rotator>();
					if (descriptionLines.size() >= 1)
					{
						comp->SetEnabled(descriptionLines[1] == "true" ? true : false);
						comp->Deserialize(descriptionLines);
					}
				}
				descriptionLines.clear();
			}
			else if (strcmp(lineData, "BeginRenderer") == 0)
			{
				descriptionLines.clear();
			}
			else if (strcmp(lineData, "EndRenderer") == 0)
			{
				m_renderer->Deserialize(descriptionLines);
				descriptionLines.clear();
			}
			else if (strcmp(lineData, "val") == 0)
			{
				std::string value = m_serializedVector[i];
				value.erase(0, 4);
				descriptionLines.push_back(value);
			}
		}
	}
	m_initialVector = m_serializedVector;
#pragma warning(pop)
}

void Scene::Serialize()
{
	m_serializedString = "BeginScene\nval ";
	m_serializedString += GetName();
	m_serializedString += "\nEndScene";
	m_serializedString += "\n\n";
	m_serializedString += m_renderer->Serialize();
	m_serializedString += "\n\n";
	m_serializedString += m_ecs->Serialize();
	m_serializedVector.clear();
	std::istringstream iss(m_serializedString);
	for (std::string line; std::getline(iss, line); )
	{
		if (line != "\n")
		m_serializedVector.push_back(line);
	}
	m_initialVector = m_serializedVector;
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
