#include "core.h"
#include <stdio.h>
#include <direct.h>

#include <GL/glew.h>
#include <glfw3.h>

#include "asset/AssetManager.h"
#include "ecs/ECS.h"
#include "render/SimpleRenderer.h"
#include "ecs/Components.h"

namespace Crimson
{
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void window_close_callback(GLFWwindow* w)
	{
		glfwSetWindowShouldClose(w, true);
	}

	Renderer renderer;

	void Test()
	{
		GLFWwindow * window;

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
		glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
		glfwSwapInterval(1);



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

		/*glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);*/

		renderer = SimpleRenderer();
		EcsSystem ecs;
		ecs.NewEntity("ENTITY");
		MeshRenderer * mr = &ecs.LastEntity()->AttachComponent<MeshRenderer>();
		mr->SetRenderer(&renderer);
		mr->SetMesh("cube");
		mr->SetShader("shader");

		glViewport(0, 0, 1280, 720);
		renderer.Flush();
		glfwSwapBuffers(window);

		int col = 0;

		while (!glfwWindowShouldClose(window))
		{
			std::cout << "L00P" << std::endl;

			glViewport(0, 0, 1280, 720);

			for (auto& ent : ecs.entities) ent->Render();

			if (col == 0)
			{
				renderer.SetClearColor(0.0f, 0.0f, 0.0f);
				col = 1;
			}
			else
			{
				renderer.SetClearColor(1.0f, 1.0f, 1.0f);
				col = 0;
			}

			renderer.Flush();

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
	}
}