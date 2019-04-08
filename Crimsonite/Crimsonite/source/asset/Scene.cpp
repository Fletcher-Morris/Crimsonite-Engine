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

	{
		m_ecs->NewEntity("MainCamera");
		Camera * mainCamera = &m_ecs->NewestEntity()->AttachComponent<Camera>();
		mainCamera->entity->MakeImmortal(true);
		mainCamera->SetRenderer(m_renderer);
		AssetManager::Instance()->CreateFrameBuffer("MainCamBuffer", Window::Width(), Window::Height());
		mainCamera->SetOutputFrameBuffer("MainCamBuffer");
		m_ecs->NewEntity("DRAGON");
		m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetRenderer(m_renderer);
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMesh("dragon");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMaterial("room");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().entity->transform.SetPosition(0, 0, -2.5);
		m_ecs->NewestEntity()->AttachComponent<Rotator>();
		m_ecs->NewEntity("CUBE");
		m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetRenderer(m_renderer);
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMesh("cube");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMaterial("crimsontex");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().entity->transform.SetPosition(0.8, 0, -1.2);
		m_ecs->NewestEntity()->AttachComponent<Rotator>();
		m_ecs->NewEntity("SPRING");
		m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetRenderer(m_renderer);
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMesh("knot");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMaterial("flat");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().entity->transform.SetPosition(-0.8, 0, -1.2);
		m_ecs->NewestEntity()->AttachComponent<Rotator>();
	}
}

Scene::Scene(std::string _scenePath)
{
	m_renderer = new SimpleRenderer();
	m_ecs = new EcsSystem();

	{
		m_ecs->NewEntity("MainCamera");
		Camera * mainCamera = &m_ecs->NewestEntity()->AttachComponent<Camera>();
		mainCamera->entity->MakeImmortal(true);
		mainCamera->SetRenderer(m_renderer);
		AssetManager::Instance()->CreateFrameBuffer("MainCamBuffer", Window::Width(), Window::Height());
		mainCamera->SetOutputFrameBuffer("MainCamBuffer");
		m_ecs->NewEntity("DRAGON");
		m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetRenderer(m_renderer);
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMesh("dragon");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMaterial("room");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().entity->transform.SetPosition(0, 0, -2.5);
		m_ecs->NewestEntity()->AttachComponent<Rotator>();
		m_ecs->NewEntity("CUBE");
		m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetRenderer(m_renderer);
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMesh("cube");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMaterial("crimsontex");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().entity->transform.SetPosition(0.8, 0, -1.2);
		m_ecs->NewestEntity()->AttachComponent<Rotator>();
		m_ecs->NewEntity("SPRING");
		m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetRenderer(m_renderer);
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMesh("knot");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMaterial("flat");
		m_ecs->NewestEntity()->GetComponent<MeshRenderer>().entity->transform.SetPosition(-0.8, 0, -1.2);
		m_ecs->NewestEntity()->AttachComponent<Rotator>();
	}

	m_path = _scenePath;

	Reload(_scenePath);
}

void Scene::Reload(std::string _scenePath)
{
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
		}
		else if (strcmp(lineData, "BeginComponent") == 0)
		{
			descType = DESCRIBING_COMPONENT;
		}
		else if (strcmp(lineData, "EndComponent") == 0)
		{
			descType = DESCRIBING_NULL;
		}
		else if (strcmp(lineData, "BeginRenderer") == 0)
		{
			descType = DESCRIBING_RENDERER;
			std::cout << "Began describing scene renderer" << std::endl;
		}
		else if (strcmp(lineData, "EndRenderer") == 0)
		{
			descType = DESCRIBING_NULL;
			std::cout << "Ended describing scene renderer" << std::endl;
		}
		else
		{
			if (descType != prevType)
			{
				if (descType == 0)
				{
					descriptionLines.clear();
				}
			}
			else
			{
				fscanf(file, "%s", &lineData);
				descriptionLines.push_back((std::string)lineData);
				std::cout << "LINE DATA :" << std::endl;
				std::cout << (std::string)lineData << std::endl;
			}
		}
	}
#pragma warning(pop)
}

void Scene::Reload()
{
	Reload(m_path);
}

void Scene::Deserialize()
{
}

void Scene::Serialize()
{
	m_serializedString = "SceneName " + GetName();
	m_serializedString += "\n";
	m_serializedString += m_renderer->Serialize();
	m_serializedString += "\n";
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
