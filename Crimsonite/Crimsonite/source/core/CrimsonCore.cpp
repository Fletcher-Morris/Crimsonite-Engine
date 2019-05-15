#include <direct.h>

#include "CrimsonCore.h"

#include "../editor/Editor.h"

void GlfwFrameBufferSizeCallback(GLFWwindow * _window, int _width, int _height);
void GlfwKeyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods);
void GlfwMousePosCallback(GLFWwindow * _window, double _x, double _y);
void GlfwMouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods);
void GlfwFileDropCallback(GLFWwindow * _widow, int _count, const char** _paths);

CrimsonCore::CrimsonCore()
{
	CrimsonCore("Crimsonite Engine");
}

CrimsonCore::CrimsonCore(std::string _appName)
{
	std::cout << "==========" << std::endl;
	printf("CRIMSONITE\n");
	std::cout << "==========" << std::endl;

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
	m_monitor = glfwGetPrimaryMonitor();
	m_videoMode = glfwGetVideoMode(m_monitor);
	m_window = glfwCreateWindow(1600, 900, _appName.c_str(), NULL, NULL);
	//m_window = glfwCreateWindow(m_videoMode->width, m_videoMode->height, _appName.c_str(), m_monitor, NULL);
	if (!m_window)
	{
		std::cout << "Failed to create GLFW window!" << std::endl; return;
	}
	Window::SetCore(this);
	Window::UpdateGlfwMode(m_videoMode->width, m_videoMode->height, m_videoMode->refreshRate);
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, GlfwFrameBufferSizeCallback);
	glfwSetKeyCallback(m_window, GlfwKeyCallback);
	glfwSetCursorPosCallback(m_window, GlfwMousePosCallback);
	glfwSetMouseButtonCallback(m_window, GlfwMouseButtonCallback);
	glfwSetDropCallback(m_window, GlfwFileDropCallback);
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

void CrimsonCore::SetWindowFullscreen(bool _fullscreen)
{
	if (_fullscreen)
	{
		int x;
		int y;
		glfwGetWindowPos(m_window, &x, &y);
		Window::SetTransform(m_videoMode->width, m_videoMode->height, x, y);
		glfwSetWindowMonitor(m_window, m_monitor, 0, 0, m_videoMode->width, m_videoMode->height, m_videoMode->refreshRate);
	}
	else
	{
		WindowTransform transform = Window::Transform();
		glfwSetWindowMonitor(m_window, NULL, 0, 0, transform.width, transform.height, m_videoMode->refreshRate);
		glfwSetWindowPos(m_window, transform.posX, transform.posY);
	}
	std::vector<Camera*> cameras = Scene::Current()->ECS()->GetAllComponentsOfType<Camera>();
	for (int i = 0; i < cameras.size(); i++)
	{
		Camera * cam = cameras[i];
		if (cam->AutoResize() && cam->entity->GetName() != "EditorCamera")
			cam->SetCameraSize(m_videoMode->width, m_videoMode->height, "core");
	}
}

void CrimsonCore::RunEngine()
{

	glfwWindowHint(GLFW_SAMPLES, 4);

	AssetManager::SetEngine(this);

	AssetManager::LoadTexture("noise", m_assetPath + "textures/noise.png");
	AssetManager::LoadTexture("crimsontex", m_assetPath + "textures/crimsontex.png");
	AssetManager::LoadTexture("room", m_assetPath + "textures/room.png");
	AssetManager::LoadTexture("flat", m_assetPath + "textures/flat.png");

	AssetManager::LoadMesh("quad", m_assetPath + "meshes/quad");
	AssetManager::LoadMesh("cube", m_assetPath + "meshes/cube");
	AssetManager::LoadMesh("sphere", m_assetPath + "meshes/sphere");
	AssetManager::LoadMesh("dragon", m_assetPath + "meshes/dragon");
	AssetManager::LoadMesh("spring", m_assetPath + "meshes/spring");
	AssetManager::LoadMesh("knot", m_assetPath + "meshes/knot");
	AssetManager::LoadMesh("teapot", m_assetPath + "meshes/teapot");

	AssetManager::LoadShader("color", m_assetPath + "shaders/vertex.vert", m_assetPath + "shaders/fragment.frag");
	AssetManager::CreatePassthroughShader();

	AssetManager::LoadMaterial(m_assetPath + "materials/crimsontex");
	AssetManager::LoadMaterial(m_assetPath + "materials/room");
	AssetManager::LoadMaterial(m_assetPath + "materials/flat");

	AssetManager::LoadScene(m_assetPath + "scenes/scene1");

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
		if (m_editor != NULL) m_editor->Update();
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
	glViewport(0, 0, _width, _height);
	Window::UpdateGlfwMode(_width, _height);
	std::vector<Camera*> cameras = Scene::Current()->ECS()->GetAllComponentsOfType<Camera>();
	for (int i = 0; i < cameras.size(); i++)
	{
		Camera * cam = cameras[i];
		if (cam->AutoResize() && cam->entity->GetName() != "EditorCamera")
			cam->SetCameraSize((int)_width, (int)_height, "core");
	}
}
void GlfwKeyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods)
{
	Input::SetKeyState(_key, _action);
}
void GlfwMousePosCallback(GLFWwindow * _window, double _x, double _y)
{
	Input::SetMousePos(_x, _y);
}
void GlfwMouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods)
{
	Input::SetMouseButton(_button, _action);
}

void GlfwFileDropCallback(GLFWwindow * _widow, int _count, const char ** _paths)
{
#if _DEBUG
	for (int i = 0; i < _count; i++)
	{
		std::string path = std::string(_paths[i]);
		if (path.find(".obj") != std::string::npos)
		{
			for (int j = 0; j < 4; j++)
			{
				path.pop_back();
			}
			AssetManager::LoadMesh(path, path);
		}
		else if (path.find(".mesh") != std::string::npos)
		{
			for (int j = 0; j < 5; j++)
			{
				path.pop_back();
			}
			AssetManager::LoadMesh(path, path);
		}
		else if (path.find(".png") != std::string::npos)
		{
			for (int j = 0; j < 4; j++)
			{
				path.pop_back();
			}
			AssetManager::LoadTexture(path, path);
		}
		else if (path.find(".jpg") != std::string::npos)
		{
			for (int j = 0; j < 4; j++)
			{
				path.pop_back();
			}
			AssetManager::LoadTexture(path, path);
		}
		else if (path.find(".material") != std::string::npos)
		{
			for (int j = 0; j < 9; j++)
			{
				path.pop_back();
			}
			AssetManager::LoadMaterial(path);
		}
		else if (path.find(".crimsn") != std::string::npos)
		{
			for (int j = 0; j < 7; j++)
			{
				path.pop_back();
			}
			AssetManager::LoadScene(path);
		}
	}
#endif // DEBUG
}
