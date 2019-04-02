#include "Editor.h"

#include "../asset/AssetManager.h"

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"

#include "../core/CrimsonCore.h"


CrimsonCore * m_engine;

Editor::Editor(CrimsonCore * _core)
{
	m_engine = _core;
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

	AssetManager::Instance()->CreateFrameBuffer("viewport", m_engine->GetVideoMode()->width, m_engine->GetVideoMode()->height);

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = false;
	io.ConfigWindowsResizeFromEdges = true;
}

void Editor::LoadIcons()
{
	AssetManager::Instance()->LoadTexture("editor_tool_move", m_engine->AssetsPath() + "editor/tool_move.png");
	AssetManager::Instance()->LoadTexture("editor_tool_move_selected", m_engine->AssetsPath() + "editor/tool_move_selected.png");
	AssetManager::Instance()->LoadTexture("editor_tool_rotate", m_engine->AssetsPath() + "editor/tool_rotate.png");
	AssetManager::Instance()->LoadTexture("editor_tool_rotate_selected", m_engine->AssetsPath() + "editor/tool_rotate_selected.png");
	AssetManager::Instance()->LoadTexture("editor_tool_scale", m_engine->AssetsPath() + "editor/tool_scale.png");
	AssetManager::Instance()->LoadTexture("editor_tool_scale_selected", m_engine->AssetsPath() + "editor/tool_scale_selected.png");
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
			if (ImGui::Button("New Scene"))
			{

			}
			if (ImGui::Button("Open Scene"))
			{

			}
			if (ImGui::Button("Save Scene"))
			{
				SaveScene();
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
				for (int i = 0; i < AssetManager::Instance()->TextureCount(); i++)
				{
					ImGui::LabelText(AssetManager::Instance()->GetTexture(i)->GetName().c_str(),"");
					if (ImGui::ImageButton((GLuint*)AssetManager::Instance()->GetTexture(i)->TextureId, ImVec2(50.0f, 50.0f), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128))) {}
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Models"))
			{
				for (int i = 0; i < AssetManager::Instance()->MeshCount(); i++)
				{
					if (ImGui::Button(AssetManager::Instance()->GetMeshName(i).c_str()))
					{

					}
				}

				ImGui::EndMenu();
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
			if (ImGui::ImageButton((GLuint*)AssetManager::Instance()->GetTexture("editor_tool_move_selected")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255))) {}
		}
		else
		{
			if (ImGui::ImageButton((GLuint*)AssetManager::Instance()->GetTexture("editor_tool_move")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255)))
			{
				SelectEditorTool(TOOL_MOVE);
			}
		}

		ImGui::SameLine();
		if (m_selectedTool == TOOL_ROTATE)
		{
			if (ImGui::ImageButton((GLuint*)AssetManager::Instance()->GetTexture("editor_tool_rotate_selected")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255))) {}
		}
		else
		{
			if (ImGui::ImageButton((GLuint*)AssetManager::Instance()->GetTexture("editor_tool_rotate")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255)))
			{
				SelectEditorTool(TOOL_ROTATE);
			}
		}
		
		ImGui::SameLine();
		if (m_selectedTool == TOOL_SCALE)
		{
		if (ImGui::ImageButton((GLuint*)AssetManager::Instance()->GetTexture("editor_tool_scale_selected")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255))) {}
		}
		else
		{
			if (ImGui::ImageButton((GLuint*)AssetManager::Instance()->GetTexture("editor_tool_scale")->TextureId, ImVec2(35.0f, 35.0f), ImVec2(0, 0), ImVec2(1, 1), 0, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255)))
			{ SelectEditorTool(TOOL_SCALE); }
		}
	}
	ImGui::End();

	ImGui::Begin("Viewport");
	{
		ImGui::Image((GLuint*)AssetManager::Instance()->GetTexture("viewport")->TextureId, ImVec2(1280, 720));
	}
	ImGui::End();

	ImGui::Begin("ENTITIES");
	{
		for (int i = 0; i < m_engine->ECS()->EntityCount(); i++)
					{
						EcsEntity * entity = &*m_engine->ECS()->entities[i];

						if (entity->IsDestroyed() == false)
						{
							if (ImGui::Button(entity->GetName().c_str()))
							{
								entity->Destroy();
							}
						}
					}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::SaveScene()
{
}

void Editor::PlayGame()
{
}

void Editor::PauseGame()
{
}

void Editor::StopGame()
{
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
