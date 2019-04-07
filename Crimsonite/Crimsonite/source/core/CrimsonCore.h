#pragma once

#include <string>
#include <iostream>

#include <gl/glew.h>
#include <glfw3.h>

#include "Window.h"

#include "../ecs/ECS.h"
#include "../ecs/Components.h"
#include "../asset/AssetManager.h"
#include "../render/SimpleRenderer.h"

class Editor;

class CrimsonCore
{

public:

	CrimsonCore();
	CrimsonCore(std::string _appName);

	void InitializeEngine(std::string _appName);
	void InitializeGlfw(std::string _appName);
	void InitializeGlew();

	void RunEngine();
	void QuitEngine();

	std::string AssetsPath() { return m_assetPath; }

	Scene * GetCurrentScene() { return m_currentScene; }

private:


	//	Application Settings
	std::string m_appName = "Crimsonite Engine";


	bool m_quit = false;

	GLFWwindow * m_window;
	GLFWmonitor * m_monitor;
	const GLFWvidmode * m_videoMode;

	Editor * m_editor;
	AssetManager * Assets;

	std::string m_assetPath = "";

	Scene * m_currentScene;

};