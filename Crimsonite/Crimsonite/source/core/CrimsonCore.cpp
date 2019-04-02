#include <direct.h>

#include "CrimsonCore.h"

#include "../editor/Editor.h"

void GlfwFrameBufferSizeCallback(GLFWwindow * _window, int _width, int _height);
EcsSystem * ecsSystem;

CrimsonCore::CrimsonCore()
{
	CrimsonCore("Crimsonite Engine");
}

CrimsonCore::CrimsonCore(std::string _appName)
{
#if _DEBUG
	m_editor = new Editor(this);
#endif // DEBUG

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

	m_renderer = new SimpleRenderer();
	m_ecs = new EcsSystem();
	ecsSystem = m_ecs;
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
	m_window = glfwCreateWindow(m_videoMode->width, m_videoMode->height, _appName.c_str(), m_monitor, NULL);
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

	Assets->LoadShader("color", m_assetPath + "shaders/vertex.vert", m_assetPath + "shaders/fragment.frag");
	Assets->CreatePassthroughShader();

	Assets->LoadMaterial(m_assetPath + "materials/crimsontex");
	Assets->LoadMaterial(m_assetPath + "materials/room");
	Assets->LoadMaterial(m_assetPath + "materials/flat");

	m_ecs->NewEntity("Camera");
	Camera * mainCamera = &m_ecs->NewestEntity()->AttachComponent<Camera>();
	mainCamera->entity->MakeImmortal(true);
	mainCamera->SetRenderer(m_renderer);

	m_ecs->NewEntity("DRAGON");
	m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetRenderer(m_renderer);
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMesh("dragon");
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMaterial("room");
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().entity->transform.SetPosition(0, 0, -2.5);
	m_ecs->NewestEntity()->AttachComponent<Rotator>();

	m_ecs->NewEntity("CUBE");
	m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetRenderer(m_renderer);
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMesh("cube");
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMaterial("crimsontex");
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().entity->transform.SetPosition(0.8, 0, -1.2);
	m_ecs->NewestEntity()->AttachComponent<Rotator>();

	m_ecs->NewEntity("SPRING");
	m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetRenderer(m_renderer);
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMesh("knot");
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().SetMaterial("flat");
	m_ecs->NewestEntity()->GetComponent<MeshRenderer>().entity->transform.SetPosition(-0.8, 0, -1.2);
	m_ecs->NewestEntity()->AttachComponent<Rotator>();


	if (m_editor != NULL)
	{
		m_editor->Init();
		mainCamera->SetOutputFrameBuffer("viewport");
	}


	while (!glfwWindowShouldClose(m_window) && m_quit == false)
	{
		Time::SetFrameTime(glfwGetTime());

		for (auto& ent : m_ecs->entities)ent->Update();
		for (auto& ent : m_ecs->entities)ent->Render();

		m_renderer->Proccess();
		m_renderer->Flush();

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

void GlfwFrameBufferSizeCallback(GLFWwindow * _window, int _width, int _height)
{
	glViewport(0, 0, _width, _height);

	if (ecsSystem->FindEntity("Camera"))
	{
		ecsSystem->FindEntity("Camera")->GetComponent<Camera>().SetCameraSettings(_width, _height);
	}
}
