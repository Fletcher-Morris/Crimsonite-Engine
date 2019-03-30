#include "Editor.h"

#include "../asset/AssetManager.h"

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"


Editor::Editor(GLFWwindow * _window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 130");


	std::cout << "Initialized Editor" << std::endl;
}

void Editor::DrawGui()
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Assets"))
		{
			if (ImGui::BeginMenu("Textures"))
			{
				for (int i = 0; i < AssetManager::Instance()->TextureCount(); i++)
				{
					AssetManager::Instance()->GetTexture(i)->Bind();
					if (ImGui::ImageButton((GLuint*)i, ImVec2(50.0f, 50.0f), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128))) {}
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
		ImGui::EndMainMenuBar();
	}


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Save()
{
}

void Editor::Play()
{
}

void Editor::Pause()
{
}

void Editor::Stop()
{
}
