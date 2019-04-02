#pragma once

#include <string>
#include <iostream>

#include <gl/glew.h>
#include <glfw3.h>

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

	GLFWwindow * GetWindow() { return m_window; }
	GLFWmonitor * GetMonitor() { return m_monitor; }
	const GLFWvidmode * GetVideoMode() { return m_videoMode; }

	void RunEngine();
	void QuitEngine();


	EcsSystem * ECS() { return m_ecs; }

	std::string AssetsPath() { return m_assetPath; }
	Renderer * m_renderer;

private:


	//	Application Settings
	std::string m_appName = "Crimsonite Engine";


	bool m_quit = false;

	GLFWwindow * m_window;
	GLFWmonitor * m_monitor;
	const GLFWvidmode * m_videoMode;

	Editor * m_editor;
	AssetManager * Assets;
	EcsSystem * m_ecs;

	std::string m_assetPath = "";

};