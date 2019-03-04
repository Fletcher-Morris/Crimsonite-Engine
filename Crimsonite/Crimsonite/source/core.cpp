#include "core.h"
#include <stdio.h>
#include <direct.h>

#include <GL/glew.h>
#include <glfw3.h>

#include "asset/AssetManager.h"
#include "ecs/ECS.h"

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



		AssetManager::Instance()->LoadMesh("cube", "C:/Users/fmorris/Documents/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/cube");
		std::cout << AssetManager::Instance()->GetMesh("cube")->IndexCount() << " indices" << std::endl;
		AssetManager::Instance()->WriteMeshFile(*AssetManager::Instance()->GetMesh("cube"), "C:/Users/fmorris/Documents/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/cube2.mesh");

		AssetManager::Instance()->LoadShader("shader", "C:/Users/fmorris/Documents/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/vertex.vert", "C:/Users/fmorris/Documents/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/fragment.frag");


		EcsSystem ecs;
		ecs.NewEntity("ENTITY");
		ecs.DestroyEntity("ENTITY");

		while (true) {};
	}
}