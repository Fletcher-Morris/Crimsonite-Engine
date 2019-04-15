#include "Editor.h"

#include "../asset/AssetManager.h"

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"

#include "../core/CrimsonCore.h"


CrimsonCore * m_engine;

Editor::Editor(CrimsonCore * _core)
{
	m_engine = _core;
	Init();
}

void Editor::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_engine->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
	std::cout << "Initialized Editor" << std::endl;

	LoadIcons();

	AssetManager::CreateFrameBuffer("EditorViewport", m_engine->GetVideoMode()->width, m_engine->GetVideoMode()->height);

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = false;
	io.ConfigWindowsResizeFromEdges = true;
}

void Editor::CreateEditorCam()
{
	m_editorCam = &m_currentScene->ECS()->NewEntity("EditorCam").AttachComponent<Camera>();
	m_editorCam->SetOutputFrameBuffer("EditorViewport");
	m_editorCam->entity->MakeImmortal(true);
	m_editorCam->entity->SetSerializable(false);
	m_editorCam->SetRenderer(m_engine->GetCurrentScene()->Renderer());
}

void Editor::LoadIcons()
{
	AssetManager::LoadTexture("editor_tool_move", m_engine->AssetsPath() + "editor/tool_move.png");
	AssetManager::LoadTexture("editor_tool_move_selected", m_engine->AssetsPath() + "editor/tool_move_selected.png");
	AssetManager::LoadTexture("editor_tool_rotate", m_engine->AssetsPath() + "editor/tool_rotate.png");
	AssetManager::LoadTexture("editor_tool_rotate_selected", m_engine->AssetsPath() + "editor/tool_rotate_selected.png");
	AssetManager::LoadTexture("editor_tool_scale", m_engine->AssetsPath() + "editor/tool_scale.png");
	AssetManager::LoadTexture("editor_tool_scale_selected", m_engine->AssetsPath() + "editor/tool_scale_selected.png");
	AssetManager::LoadTexture("editor_tool_play", m_engine->AssetsPath() + "editor/tool_play.png");
	AssetManager::LoadTexture("editor_tool_play_selected", m_engine->AssetsPath() + "editor/tool_play_selected.png");
	AssetManager::LoadTexture("editor_tool_stop", m_engine->AssetsPath() + "editor/tool_stop.png");
	AssetManager::LoadTexture("editor_tool_stop_selected", m_engine->AssetsPath() + "editor/tool_stop_selected.png");
}

void Editor::CreateObject(std::string _meshName)
{
	m_currentScene->ECS()->NewEntity(_meshName);
	MeshRenderer * mesh = &m_currentScene->ECS()->NewestEntity()->AttachComponent<MeshRenderer>();
	mesh->SetMesh(_meshName);
	mesh->SetRenderer(m_engine->GetCurrentScene()->Renderer());
	mesh->SetMaterial("default");
	mesh->entity->transform.SetPosition(0, 0, -2);
	mesh->entity->AttachComponent<Rotator>();
}

void Editor::DrawGui()
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (m_engine->GetPlayMode() == PLAYMODE_STOPPED)
			{
				if (ImGui::Button("New Scene"))
				{
					CreateAndLoadNewScene("New Scene");
				}
				if (ImGui::Button("Open Scene"))
				{

				}
				if (ImGui::Button("Save Scene"))
				{
					m_currentScene->Save();
				}
				if (ImGui::Button("Reload Scene"))
				{
					m_currentScene->Reload();
					CreateEditorCam();
				}
			}
			if (ImGui::Button("Quit"))
			{
				Quit();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Assets"))
		{
			if (ImGui::BeginMenu("Textures"))
			{
				for (int i = 0; i < AssetManager::TextureCount(); i++)
				{
					ImGui::Text("%s", AssetManager::GetTexture(i)->GetName().c_str());
					ImGui::Text("%i X %i", AssetManager::GetTexture(i)->GetWidth(), AssetManager::GetTexture(i)->GetHeight());
					ImGui::Text("GL ID : %i", AssetManager::GetTexture(i)->TextureId);
					if (ImGui::ImageButton((GLuint*)AssetManager::GetTexture(i)->TextureId, ImVec2(50.0f, 50.0f), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128))) {}
					ImGui::Separator();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Models"))
			{
				for (int i = 0; i < AssetManager::MeshCount(); i++)
				{
					if (ImGui::Button(AssetManager::GetMeshName(i).c_str()))
					{

					}
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::Button("Cube"))
			{
				CreateObject("cube");
			}
			if (ImGui::Button("Sphere"))
			{
				CreateObject("sphere");
			}
			if (ImGui::Button("Quad"))
			{
				CreateObject("quad");
			}
			if (ImGui::Button("Knot"))
			{
				CreateObject("knot");
			}
			if (ImGui::Button("Dragon"))
			{
				CreateObject("dragon");
			}
			if (ImGui::Button("Teapot"))
			{
				CreateObject("teapot");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::Button("Toolbar"))
			{
			}
			if (ImGui::Button("Viewport"))
			{
			}
			if (ImGui::Button("Entities"))
			{
			}
			if (ImGui::Button("Properties"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scene"))
		{
			ImGui::InputText("", m_tempSceneName, 128);
			ImGui::SameLine();
			if (ImGui::Button("Rename"))
			{
				m_currentScene->SetName((std::string)m_tempSceneName);
			}
			ImGui::EndMenu();
		}

		ImGui::Separator();
		ImGui::SameLine(ImGui::GetWindowWidth() - (80));
		ImGui::Text("%i FPS",Time::GetFps());

		ImGui::EndMainMenuBar();
	}
	
	ImGui::Begin("Toolbar");
	{
		ImGui::SameLine();
		if (m_selectedTool == TOOL_MOVE)
		{
			if (ImGui::ImageButton((GLuint*)AssetManager::GetTexture("editor_tool_move_selected")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255))) {}
		}
		else
		{
			if (ImGui::ImageButton((GLuint*)AssetManager::GetTexture("editor_tool_move")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255)))
			{
				SelectEditorTool(TOOL_MOVE);
			}
		}
		ImGui::SameLine();
		if (m_selectedTool == TOOL_ROTATE)
		{
			if (ImGui::ImageButton((GLuint*)AssetManager::GetTexture("editor_tool_rotate_selected")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255))) {}
		}
		else
		{
			if (ImGui::ImageButton((GLuint*)AssetManager::GetTexture("editor_tool_rotate")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255)))
			{
				SelectEditorTool(TOOL_ROTATE);
			}
		}		
		ImGui::SameLine();
		if (m_selectedTool == TOOL_SCALE)
		{
		if (ImGui::ImageButton((GLuint*)AssetManager::GetTexture("editor_tool_scale_selected")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255))) {}
		}
		else
		{
			if (ImGui::ImageButton((GLuint*)AssetManager::GetTexture("editor_tool_scale")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255)))
			{ SelectEditorTool(TOOL_SCALE); }
		}

		if (m_engine->GetPlayMode() == PLAYMODE_STOPPED)
		{

			ImGui::SameLine(ImGui::GetWindowWidth()/2);
			if (ImGui::ImageButton((GLuint*)AssetManager::GetTexture("editor_tool_play")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255)))
			{
				PlayGame();
			}
		}
		else
		{
			ImGui::SameLine(ImGui::GetWindowWidth() / 2);
			if (ImGui::ImageButton((GLuint*)AssetManager::GetTexture("editor_tool_stop")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255)))
			{
				StopGame();
			}
		}
	}
	ImGui::End();

	ImGui::Begin("Viewport");
	{
		ImGui::Image((GLuint*)AssetManager::GetTexture("EditorViewport")->TextureId, ImVec2(1280, 720));
	}
	ImGui::End();

	ImGui::Begin("Entities");
	{
		for (int i = 0; i < m_currentScene->ECS()->EntityCount(); i++)
		{
			EcsEntity * entity = &*m_currentScene->ECS()->entities[i];

			if (entity->IsDestroyed() == false)
			{
				if (ImGui::Button(entity->GetName().c_str()))
				{
					m_selectedEditorObject = entity;
				}
			}
		}
	}
	ImGui::End();

	ImGui::Begin("Properties");
	{
		if (!m_selectedEditorObject)
		{
			m_selectedEditorObject = m_editorCam->entity;
		}
		m_selectedEditorObject->DrawEditorProperties();
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Update()
{
	glm::vec3 moveVector = glm::vec3();
	if (Input::GetKey(KEYCODE_RIGHT)) moveVector.x = 1;
	else if (Input::GetKey(KEYCODE_LEFT)) moveVector.x = -1;
	if (Input::GetKey(KEYCODE_RIGHT_SHIFT)) moveVector.y = 1;
	else if (Input::GetKey(KEYCODE_RIGHT_CONTROL)) moveVector.y = -1;
	if (Input::GetKey(KEYCODE_UP)) moveVector.z = 1;
	else if (Input::GetKey(KEYCODE_DOWN)) moveVector.z = -1;
	m_editorCam->entity->transform.Move(moveVector * Time::DeltaTime());
}

void Editor::PlayGame()
{
	m_engine->GetCurrentScene()->Serialize();
	m_engine->SetPlayMode(PLAYMODE_RUNNING);
}

void Editor::PauseGame()
{
	m_engine->SetPlayMode(PLAYMODE_PAUSED);
}

void Editor::StopGame()
{
	m_storedEdCamPos = m_editorCam->entity->transform.GetPosition();
	m_storedEdCamRot = m_editorCam->entity->transform.GetRotation();
	if (m_selectedEditorObject->GetTypeString() == "EcsEntity")
	{
		EcsEntity * ent = static_cast<EcsEntity*>(m_selectedEditorObject);
		m_selectedEntityName = ent->GetName();
	}
	else { m_selectedEntityName = ""; }
	m_engine->SetPlayMode(PLAYMODE_STOPPED);
	m_engine->GetCurrentScene()->Deserialize();
	CreateEditorCam();
	m_editorCam->entity->transform.SetPosition(m_storedEdCamPos);
	m_editorCam->entity->transform.SetRotation(m_storedEdCamRot);
	if (m_selectedEntityName != "")
	{
		m_selectedEditorObject = m_engine->GetCurrentScene()->ECS()->FindEntity(m_selectedEntityName);
	}
	else if (!m_selectedEditorObject) { m_selectedEditorObject = m_editorCam->entity; }
}

void Editor::Quit()
{
	m_engine->QuitEngine();
}

void Editor::SelectEditorTool(int _tool)
{
	m_selectedTool = _tool;
	std::cout << "Selected editor tool '" << m_selectedTool << "'." << std::endl;
}

void Editor::SetCurrentSceneData(Scene * _scene)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	std::strcpy(m_tempSceneName, _scene->GetName().c_str());
#pragma warning(pop)
	m_currentScene = _scene;
}

void Editor::CreateAndLoadNewScene(std::string _sceneName)
{
	Scene newScene = Scene();
	newScene.SetPath(m_engine->AssetsPath() + "scenes/NewScene");
	newScene.Save();
	AssetManager::LoadScene(m_engine->AssetsPath() + "scenes/NewScene");
	AssetManager::OpenScene("NewScene");
}
