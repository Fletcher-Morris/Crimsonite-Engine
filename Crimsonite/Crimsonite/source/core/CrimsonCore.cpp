#include <direct.h>

#include "CrimsonCore.h"

#include "../editor/Editor.h"

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

	Assets = AssetManager::Instance();
	m_assetPath = (std::string)_getcwd(NULL, 0) + "/assets/";
	//m_assetPath = "G:/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/";
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
	m_monitor = glfwGetPrimaryMonitor();
	m_videoMode = glfwGetVideoMode(m_monitor);
	//m_window = glfwCreateWindow(1600, 900, _appName.c_str(), NULL, NULL);
	m_window = glfwCreateWindow(m_videoMode->width, m_videoMode->height, _appName.c_str(), m_monitor, NULL);
	Window::SetSize(m_videoMode->width, m_videoMode->height);
	if (!m_window)
	{
		std::cout << "Failed to create GLFW window!" << std::endl; return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, GlfwFrameBufferSizeCallback);
	glfwSwapInterval(0);
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

	glfwWindowHint(GLFW_SAMPLES, 4);

	Assets->SetEngine(this);

	Assets->LoadTexture("noise", m_assetPath + "textures/noise.png");
	Assets->LoadTexture("crimsontex", m_assetPath + "textures/crimsontex.png");
	Assets->LoadTexture("room", m_assetPath + "textures/room.png");
	Assets->LoadTexture("flat", m_assetPath + "textures/flat.png");

	Assets->LoadMesh("quad", m_assetPath + "meshes/quad");
	Assets->LoadMesh("cube", m_assetPath + "meshes/cube");
	Assets->LoadMesh("sphere", m_assetPath + "meshes/sphere");
	Assets->LoadMesh("dragon", m_assetPath + "meshes/dragon");
	Assets->LoadMesh("spring", m_assetPath + "meshes/spring");
	Assets->LoadMesh("knot", m_assetPath + "meshes/knot");
	Assets->LoadMesh("teapot", m_assetPath + "meshes/teapot");

	Assets->LoadShader("color", m_assetPath + "shaders/vertex.vert", m_assetPath + "shaders/fragment.frag");
	Assets->CreatePassthroughShader();

	Assets->LoadMaterial(m_assetPath + "materials/crimsontex");
	Assets->LoadMaterial(m_assetPath + "materials/room");
	Assets->LoadMaterial(m_assetPath + "materials/flat");

	Assets->CreateFrameBuffer("MainCamBuffer", Window::Width(), Window::Height());

	Assets->LoadScene(m_assetPath + "scenes/scene1");

#if _DEBUG
	m_editor = new Editor(this);
#endif // DEBUG

	OpenScene(0);

	while (!glfwWindowShouldClose(m_window) && m_quit == false)
	{
		Time::SetFrameTime(glfwGetTime());
		if (GetPlayMode() == PLAYMODE_RUNNING)
		{
			m_currentScene->Update();
		}
		m_currentScene->Render();
		if (m_editor != NULL) m_editor->DrawGui();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	glfwDestroyWindow(m_window);
}

void CrimsonCore::QuitEngine()
{
	m_quit = true;
}

void CrimsonCore::OpenScene(int _sceneId)
{
	Scene * openScene = Assets->GetScene(_sceneId);
	OpenScene(openScene->GetName());
}

void CrimsonCore::OpenScene(std::string _sceneName)
{
	m_currentScene = Assets->GetScene(_sceneName);
	if (m_editor)
	{
		m_editor->SetCurrentSceneData(m_currentScene);
		m_editor->CreateEditorCam();
	}
	else
	{
		SetPlayMode(PLAYMODE_RUNNING);
	}
}

void GlfwFrameBufferSizeCallback(GLFWwindow * _window, int _width, int _height)
{
	Window::SetSize(_width, _height);
}
