#include <direct.h>

#include "CrimsonCore.h"

CrimsonCore::CrimsonCore()
{
	CrimsonCore("Crimsonite Engine");
}

CrimsonCore::CrimsonCore(std::string _appName)
{
	m_assetManager = AssetManager::Instance();
	m_ecs = EcsSystem();
	m_assetPath = (std::string)_getcwd(NULL, 0) + "/assets/";
	std::cout << "Assets path is : " << m_assetPath << std::endl;
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
	m_window = glfwCreateWindow(1280, 720, _appName.c_str(), NULL, NULL);
	if (!m_window)
	{
		std::cout << "Failed to create GLFW window!" << std::endl; return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, FrameBufferSizeCallback);
	glfwSwapInterval(1);
}

void CrimsonCore::InitializeGlew()
{
}


void FrameBufferSizeCallback(GLFWwindow * _window, int _width, int _height);
void FrameBufferSizeCallback(GLFWwindow * _window, int _width, int _height)
{
	glViewport(0, 0, _width, _height);
}
