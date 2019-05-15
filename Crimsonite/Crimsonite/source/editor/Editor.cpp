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

Editor * Editor::m_instance(0);
Editor * Editor::GetEditor()
{
	return m_instance;
}

void Editor::Init()
{
	m_instance = this;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_engine->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
	std::cout << "Initialized Editor" << std::endl;

	LoadIcons();

	AssetManager::CreateFrameBuffer("EditorCamera_FrameBuffer", m_engine->GetVideoMode()->width, m_engine->GetVideoMode()->height);

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = false;
	io.ConfigWindowsResizeFromEdges = true;
}

void Editor::CreateEditorCam()
{
	if (Scene::Current()->ECS()->FindEntity("EditorCamera") == false)
	{
		m_editorCam = m_currentScene->ECS()->NewEntity("EditorCamera").AttachComponent<Camera>();
		m_editorCam->SetOutputFrameBuffer("EditorCamera_FrameBuffer");
		m_editorCam->SetAutoResize(false);
		m_editorCam->entity->MakeImmortal(true);
		m_editorCam->entity->SetSerializable(false);
		m_editorCam->SetRenderer(m_engine->GetCurrentScene()->Renderer());
		PushEditorCamTransform();
	}
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
	AssetManager::LoadTexture("editor_tool_outline", m_engine->AssetsPath() + "editor/tool_outline.png");
	AssetManager::LoadTexture("editor_tool_outline_selected", m_engine->AssetsPath() + "editor/tool_outline_selected.png");
}

void Editor::CreateObject(std::string _meshName)
{
	m_currentScene->ECS()->NewEntity(_meshName);
	MeshRenderer * mesh = m_currentScene->ECS()->NewestEntity()->AttachComponent<MeshRenderer>();
	mesh->SetMesh(_meshName);
	mesh->SetRenderer(m_engine->GetCurrentScene()->Renderer());
	mesh->SetMaterial("default");
	mesh->entity->transform.SetLocalPosition(0, 0, -2);
	mesh->entity->AttachComponent<Rotator>();
}

void Editor::PullEditorCamTransform()
{
	m_storedEdCamPos = m_editorCam->entity->transform.GetLocalPosition();
	m_storedEdCamRot = m_editorCam->entity->transform.GetLocalRotation();
}

void Editor::PushEditorCamTransform()
{
	m_editorCam->entity->transform.SetLocalPosition(m_storedEdCamPos);
	m_editorCam->entity->transform.SetLocalRotation(m_storedEdCamRot);
}

void Editor::SelectEditorObject(EditorSerializable * _editorObject)
{
	if (_editorObject == NULL) return;
	m_selectedEditorObject = _editorObject;
	std::string typeName = m_selectedEditorObject->GetTypeString();
	if (typeName == "EcsEntity")
	{
		m_selectedEntityName = static_cast<EcsEntity*>(m_selectedEditorObject)->GetName();
	}
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
				if (ImGui::BeginMenu("Open Scene"))
				{
					for (int i = 0; i < AssetManager::SceneCount(); i++)
					{
						std::string sceneName = AssetManager::GetScene(i)->GetName();
						if (ImGui::Button(sceneName.c_str()))
						{
							OpenScene(sceneName);
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::Button("Save Scene"))
				{
					SaveScene();
				}
				if (ImGui::Button("Reload Scene"))
				{
					ReloadScene();
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
			if (ImGui::BeginMenu("New Entity"))
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
			if (ImGui::Button("New Scene"))
			{
				CreateAndLoadNewScene("New Scene");
			}
			if (ImGui::BeginMenu("New Material"))
			{
				ImGui::InputText("Material Name", m_tempMaterialName, 128);

				Material * basedOnMaterial = AssetManager::GetDefaultMaterial();
				std::string basedOnMaterialName = basedOnMaterial->GetName();
				if (ImGui::BeginCombo("Based On", basedOnMaterialName.c_str()))
				{
					for (int i = 0; i < AssetManager::MaterialCount(); i++)
					{
						std::string foundMaterialName = AssetManager::GetMaterial(i)->GetName();
						bool isSelected = (basedOnMaterialName == foundMaterialName);
						if (ImGui::Button(foundMaterialName.c_str()))
						{
							basedOnMaterial = AssetManager::GetMaterial(foundMaterialName);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				Shader * chosenShader = AssetManager::GetShader("default");
				std::string chosenShaderName = chosenShader->GetName();
				if (ImGui::BeginCombo("Shader", chosenShaderName.c_str()))
				{
					for (int i = 0; i < AssetManager::ShaderCount(); i++)
					{
						std::string foundShaderName = AssetManager::GetShader(i)->GetName();
						bool isSelected = (chosenShaderName == foundShaderName);
						if (ImGui::Button(foundShaderName.c_str()))
						{
							chosenShaderName = foundShaderName;
							chosenShader = AssetManager::GetShader(chosenShaderName);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::Button("Create"))
				{
					AssetManager::AddMaterial((std::string)m_tempMaterialName, chosenShader);
					SelectEditorObject(AssetManager::GetMaterial((std::string)m_tempMaterialName));
					std::string("New Material").copy(m_tempMaterialName, 128);
				}
				ImGui::EndMenu();
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
			float * newClearColor = m_currentScene->Renderer()->GetClearColor();
			ImGui::ColorEdit3("Clear Colour", newClearColor);
			m_currentScene->Renderer()->SetClearColor(newClearColor);
			if (ImGui::Button("Auto"))
			{
				m_currentScene->Renderer()->SetOverrideMode(-1);
			}
			ImGui::SameLine();
			if (ImGui::Button("Tris"))
			{
				m_currentScene->Renderer()->SetOverrideMode(GL_TRIANGLES);
			}
			ImGui::SameLine();
			if (ImGui::Button("Lines"))
			{
				m_currentScene->Renderer()->SetOverrideMode(GL_LINES);
			}
			ImGui::SameLine();
			if (ImGui::Button("Points"))
			{
				m_currentScene->Renderer()->SetOverrideMode(GL_POINTS);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::Button("Toggle Fullscreen"))
			{
				Window::SetFullscreen(!Window::IsFullscreen());
			}
			ImGui::EndMenu();
		}

		ImGui::Separator();
		ImGui::SameLine(ImGui::GetWindowWidth() - (80));
		ImGui::Text("%i FPS",Time::GetFps());

		ImGui::EndMainMenuBar();
	}
	
	ImGui::SetNextWindowPos(ImVec2(0, 18));
	ImGui::SetNextWindowSize(ImVec2(Window::Width(), 51));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::Begin("Toolbar", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
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

	ImGui::SetNextWindowPos(ImVec2(150, 68));
	ImGui::SetNextWindowSize(ImVec2(Window::Width() - 500, Window::Height() - 68));
	ImGui::Begin("Viewport", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar| ImGuiWindowFlags_NoScrollWithMouse);
	{
		ImVec2 size = ImGui::GetWindowSize();

		if (size.x != m_prevCamSize.x || size.y != m_prevCamSize.y || size.x * size.y == 0 )
		{
			m_editorCam->SetCameraSize((int)size.x, (int)size.y, "editor");
			m_prevCamSize = size;
		}
		ImGui::Image((GLuint*)AssetManager::GetTexture("EditorCamera_FrameBuffer")->TextureId, ImVec2(size.x - 15, size.y - 15));
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 68));
	ImGui::SetNextWindowSize(ImVec2(150, Window::Height() - 68));
	ImGui::Begin("Entities", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	{
		for (int i = 0; i < m_currentScene->ECS()->EntityCount(); i++)
		{
			EcsEntity * entity = &*m_currentScene->ECS()->entities[i];
			if (entity->IsDestroyed() == false)
			{
				if (ImGui::Button(entity->GetName().c_str()))
				{
					SelectEditorObject(entity);
				}
			}
		}
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(Window::Width() - 350, 68));
	ImGui::SetNextWindowSize(ImVec2(350, Window::Height() - 68));
	ImGui::Begin("Properties", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	{
		if (!m_selectedEditorObject)
		{
			m_selectedEditorObject = m_editorCam->entity;
		}
		m_selectedEditorObject->DrawEditorProperties();
	}
	ImGui::End();

	ImGui::PopStyleVar();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Update()
{
	glm::vec3 moveVector = glm::vec3();
	if (Input::GetKey(KEYCODE_D)) moveVector.x = 1;
	else if (Input::GetKey(KEYCODE_A)) moveVector.x = -1;
	if (Input::GetKey(KEYCODE_E)) moveVector.y = 1;
	else if (Input::GetKey(KEYCODE_Q)) moveVector.y = -1;
	if (Input::GetKey(KEYCODE_W)) moveVector.z = 1;
	else if (Input::GetKey(KEYCODE_S)) moveVector.z = -1;
	m_editorCam->entity->transform.Move(moveVector * Time::DeltaTime(), true);
	if (Input::GetMouseButton(MOUSE_RIGHT))
	{
		glm::vec2 movement = Input::GetMouseMovement(1.5f);
		m_editorCam->entity->transform.Rotate(movement.y * -0.05, movement.x * 0.05, 0);
	}
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
	PullEditorCamTransform();
	if (m_selectedEditorObject->GetTypeString() == "EcsEntity")
	{
		EcsEntity * ent = static_cast<EcsEntity*>(m_selectedEditorObject);
		m_selectedEntityName = ent->GetName();
	}
	else { m_selectedEntityName = ""; }
	m_engine->SetPlayMode(PLAYMODE_STOPPED);
	m_engine->GetCurrentScene()->Deserialize();
	CreateEditorCam();
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

void Editor::SaveScene()
{
	if (m_engine->GetPlayMode() != PLAYMODE_STOPPED) return;
	if (m_currentScene)
	{
		m_currentScene->Save();
	}
}

void Editor::ReloadScene()
{
	if (m_engine->GetPlayMode() != PLAYMODE_STOPPED) return;
	PullEditorCamTransform();
	if (m_currentScene)
	{
		m_currentScene->Reload();
		CreateEditorCam();
	}
}

void Editor::OpenScene(std::string _sceneName)
{
	m_engine->OpenScene(_sceneName);
}
