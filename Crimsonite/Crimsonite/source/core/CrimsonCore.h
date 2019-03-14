#pragma once

#include <string>
#include <iostream>

#include <gl/glew.h>
#include <glfw3.h>

#include "../ecs/ECS.h"
#include "../ecs/Components.h"
#include "../asset/AssetManager.h"
#include "../render/SimpleRenderer.h"

class CrimsonCore
{

public:

	CrimsonCore();
	CrimsonCore(std::string _appName);

	void InitializeEngine(std::string _appName);
	void InitializeGlfw(std::string _appName);
	void InitializeGlew();

	void RunEngine();

private:


	//	Application Settings
	std::string m_appName = "Crimsonite Engine";


	GLFWwindow * m_window;

	Renderer * m_renderer;
	AssetManager * Assets;
	EcsSystem * m_ecs;

	std::string m_assetPath = "G:/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/";

};