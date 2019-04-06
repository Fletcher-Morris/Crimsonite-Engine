#include "Scene.h"
#include <iostream>
#include <fstream>
#include <vector>

#define DESCRIBING_NULL 0
#define DESCRIBING_ENTITY 1
#define DESCRIBING_COMPONENT 2
#define DESCRIBING_RENDERER 3

Scene::Scene(std::string _scenePath)
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

void Scene::Serialize()
{
}

std::string Scene::GetSerializedString()
{
	return m_serializedString;
}
