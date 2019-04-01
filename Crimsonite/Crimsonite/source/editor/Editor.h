#pragma once

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

class CrimsonCore;

class Editor
{

public:

	Editor(CrimsonCore * _core);
	void Init();

	void LoadIcons();

	void DrawGui();


	void SaveScene();


	void PlayGame();
	void PauseGame();
	void StopGame();


	void Quit();

};