#include "core.h"
#include <stdio.h>
#include <direct.h>

#include <GL/glew.h>
#include <glfw3.h>

#include "asset/AssetManager.h"
#include "ecs/ECS.h"
#include "render/SimpleRenderer.h"
#include "ecs/components/MeshRenderer.h"

namespace Crimson
{
	GLFWwindow * window;
	void Test()
	{
		std::cout << "==========" << std::endl;
		printf("CRIMSONITE\n");
		std::cout << "==========" << std::endl;

		std::cout << _getcwd(NULL, 0) << std::endl;


		if (!glfwInit())
		{
			std::cout << "Failed to initialise GLFW!" << std::endl;
			return;
		}
		std::cout << "Initialised GLFW (" << glfwGetVersionString() << ")" << std::endl;
		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_MULTISAMPLE);
		window = glfwCreateWindow(1280, 720, "Crimsonite Engine", NULL, NULL);
		if (!window)
		{
			std::cout << "Failed to create GLFW window!" << std::endl; return;
		}
		glfwMakeContextCurrent(window);



		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::cout << "Failed to initialise GLEW!" << std::endl;
		}
		if (err == GLEW_ERROR_NO_GL_VERSION)
		{
			std::cout << "FAILED TO INITIALISE GLEW MISSING GL VERSION" << std::endl;
		}
		std::cout << "Initialised GLEW (" << glewGetString(GLEW_VERSION) << ")" << std::endl;
		std::cout << "Initialised OpenGL (" << glGetString(GL_VERSION) << ")" << std::endl;

		std::string path = "G:/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/";

		AssetManager::Instance()->LoadMesh("cube", path + "cube");
		AssetManager::Instance()->WriteMeshFile(*AssetManager::Instance()->GetMesh("cube"), path + "cube2.mesh");
		AssetManager::Instance()->LoadShader("shader", path + "vertex.vert", path + "fragment.frag");

		Renderer renderer = SimpleRenderer();
		EcsSystem ecs;
		ecs.NewEntity("ENTITY");
		MeshRenderer * mr = &ecs.LastEntity()->AttachComponent<MeshRenderer>();
		mr->SetRenderer(&renderer);
		mr->SetMesh("cube");
		mr->SetShader("shader");

		while (true) {};
	}
}