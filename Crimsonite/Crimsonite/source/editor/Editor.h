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

private:

	char m_tempSceneName[128] = "New Scene";
	char m_tempMaterialName[128] = "New Material";

public:

	Editor(CrimsonCore * _core);
	static Editor * GetEditor();
	void Init();
	void CreateEditorCam();

	void DrawGui();
	void Update();

	void PlayGame();
	void PauseGame();
	void StopGame();

	void Quit();

	int m_selectedTool = 0;
	void SelectEditorTool(int _tool);
	void CreateObject(std::string _meshName);

	Camera * m_editorCam;
	ImVec2 m_prevCamSize;
	glm::vec3 m_storedEdCamPos;
	glm::vec3 m_storedEdCamRot;
	void PullEditorCamTransform();
	void PushEditorCamTransform();

	EditorSerializable * m_selectedEditorObject;
	void SelectEditorObject(EditorSerializable * _editorObject);
	std::string m_selectedEntityName = "";

	Scene * m_currentScene;
	void SetCurrentSceneData(Scene * _scene);
	void CreateAndLoadNewScene(std::string _sceneName);
	void SaveScene();
	void ReloadScene();
	void OpenScene(std::string _sceneName);

protected:

	static Editor * m_instance;

};