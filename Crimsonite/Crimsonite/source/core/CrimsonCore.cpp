#include <direct.h>

#include "CrimsonCore.h"

void GlfwFrameBufferSizeCallback(GLFWwindow * _window, int _width, int _height);
EcsSystem * ecsSystem;

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
	m_window = glfwCreateWindow(1280, 720, _appName.c_str(), NULL, NULL);
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

	Assets->LoadMesh("test", m_assetPath + "test");
	Assets->LoadMesh("cube", m_assetPath + "cube");
	Assets->LoadMesh("dragon", m_assetPath + "dragon");
	Assets->WriteMeshFile(*Assets->GetMesh("cube"), m_assetPath + "cube2.mesh");
	Assets->LoadMesh("spring", m_assetPath + "spring");
	Assets->LoadShader("shader", m_assetPath + "vertex.vert", m_assetPath + "fragment.frag");

	m_ecs->NewEntity("Camera");
	Camera * mainCamera = &m_ecs->NewestEntity()->AttachComponent<Camera>();

	m_ecs->NewEntity("DRAGON");
	MeshRenderer * mr = &m_ecs->NewestEntity()->AttachComponent<MeshRenderer>();
	mr->SetRenderer(m_renderer);
	mr->SetMesh("dragon");
	mr->SetShader("shader");
	mr->entity->transform.SetPosition(0, 0, -1.5);
	mr->SetRenderMode(GL_POINTS);

	while (!glfwWindowShouldClose(m_window))
	{
		for (auto& ent : m_ecs->entities)ent->Update();
		for (auto& ent : m_ecs->entities)ent->Render();

		m_renderer->Proccess();
		m_renderer->Flush();

		glfwSwapBuffers(m_window);

		glfwPollEvents();
	}
}

void GlfwFrameBufferSizeCallback(GLFWwindow * _window, int _width, int _height)
{
	glViewport(0, 0, _width, _height);

	if (ecsSystem->FindEntity("Camera"))
	{
		ecsSystem->FindEntity("Camera")->GetComponent<Camera>().SetCameraSettings(_width, _height);
	}
}
