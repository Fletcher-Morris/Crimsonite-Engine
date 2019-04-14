#pragma once

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

#include "../ecs/ECS.h"

class CrimsonCore;
class Camera;

#define TOOL_MOVE 1
#define TOOL_ROTATE 2
#define TOOL_SCALE 3

class Editor
{

public:

	Editor(CrimsonCore * _core);
	void Init();
	void CreateEditorCam();

	void LoadIcons();

	void DrawGui();

	void PlayGame();
	void PauseGame();
	void StopGame();

	void Quit();

	int m_selectedTool = 0;
	void SelectEditorTool(int _tool);

	Camera * m_editorCam;
	glm::vec3 m_storedEdCamPos;
	glm::vec3 m_storedEdCamRot;

	EditorSerializable * m_selectedEditorObject;
	std::string m_selectedEntityName = "";
	/*void SelectEntity(int _id);
	void DeselectEntity();*/

};