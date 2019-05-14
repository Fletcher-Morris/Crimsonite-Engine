#pragma once

#include <string>
#include <iostream>

#include <gl/glew.h>
#include <glfw3.h>

#include "Window.h"

#include "Input.h"
#include "../ecs/ECS.h"
#include "../ecs/Components.h"
#include "../asset/AssetManager.h"
#include "../render/SimpleRenderer.h"

class Editor;

#define PLAYMODE_STOPPED 0
#define PLAYMODE_RUNNING 1
#define PLAYMODE_PAUSED 2

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
	void SetWindowFullscreen(bool _fullscreen);

	void RunEngine();
	void QuitEngine();

	std::string AssetsPath() { return m_assetPath; }

	Scene * GetCurrentScene() { return m_currentScene; }
	void OpenScene(int _sceneId);
	void OpenScene(std::string _sceneName);

	int GetPlayMode() { return m_playMode; }
	void SetPlayMode(int _playMode) { m_playMode = _playMode; }

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

	int m_playMode = PLAYMODE_STOPPED;
};