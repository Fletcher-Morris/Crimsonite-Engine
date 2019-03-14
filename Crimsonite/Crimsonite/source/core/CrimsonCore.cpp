#include <direct.h>

#include "CrimsonCore.h"

void GlfwFrameBufferSizeCallback(GLFWwindow * _window, int _width, int _height);

CrimsonCore::CrimsonCore()
{
	CrimsonCore("Crimsonite Engine");
}

CrimsonCore::CrimsonCore(std::string _appName)
{
	std::cout << "==========" << std::endl;
	printf("CRIMSONITE\n");
	std::cout << "==========" << std::endl;

	m_assetManager = AssetManager::Instance();
	m_ecs = EcsSystem();
	m_assetPath = (std::string)_getcwd(NULL, 0) + "/assets/";
	std::cout << "Assets path is : " << m_assetPath << std::endl;

	InitializeEngine(_appName);
	RunEngine();
}

void CrimsonCore::InitializeEngine(std::string _appName)
{
	InitializeGlfw(_appName);
	InitializeGlew();
}

void CrimsonCore::InitializeGlfw(std::string _appName)
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialise GLFW!" << std::endl; return;
	}
	std::cout << "Initialised GLFW (" << glfwGetVersionString() << ")" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	m_window = glfwCreateWindow(1280, 720, _appName.c_str(), NULL, NULL);
	if (!m_window)
	{
		std::cout << "Failed to create GLFW window!" << std::endl; return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, GlfwFrameBufferSizeCallback);
	glfwSwapInterval(1);
}

void CrimsonCore::InitializeGlew()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Failed to initialise GLEW!" << std::endl;
	}
	if (err == GLEW_ERROR_NO_GL_VERSION)
	{
		std::cout << "MISSING GL VERSION" << std::endl;
	}
	std::cout << "Initialised GLEW (" << glewGetString(GLEW_VERSION) << ")" << std::endl;
	std::cout << "Initialised OpenGL (" << glGetString(GL_VERSION) << ")" << std::endl;
}

void CrimsonCore::RunEngine()
{
	m_renderer.Init();
	int col = 0;
	while (!glfwWindowShouldClose(m_window))
	{
		for (auto& ent : m_ecs.entities)ent->Update();
		for (auto& ent : m_ecs.entities)ent->Render();


		if (col == 0)
		{
			m_renderer.SetClearColor(0.0f, 0.0f, 0.0f);
			col = 1;
		}
		else
		{
			m_renderer.SetClearColor(1.0f, 1.0f, 1.0f);
			col = 0;
		}


		m_renderer.Proccess();
		m_renderer.Flush();

		glfwSwapBuffers(m_window);

		glfwPollEvents();
	}
}

void GlfwFrameBufferSizeCallback(GLFWwindow * _window, int _width, int _height)
{
	glViewport(0, 0, _width, _height);
}
