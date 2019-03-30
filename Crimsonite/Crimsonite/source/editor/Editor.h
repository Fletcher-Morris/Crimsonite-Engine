#pragma once

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

class Editor
{

public:

	Editor(GLFWwindow * _window);

	void DrawGui();


	void Save();


	void Play();
	void Pause();
	void Stop();

};