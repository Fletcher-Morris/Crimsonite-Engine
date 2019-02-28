#include "core.h"
#include <stdio.h>
#include <direct.h>

#include "asset/AssetManager.h"

namespace Crimson
{
	void Test()
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::cout << "Failed to initialise GLEW!" << std::endl;
		}
		std::cout << "Initialised GLEW (" << glewGetString(GLEW_VERSION) << ")" << std::endl;

		printf("CRIMSONITE\n");

		std::cout << _getcwd(NULL, 0) << std::endl;

		AssetManager::Instance()->LoadMesh("cube", "C:/Users/fmorris/Documents/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/cube");
		AssetManager::Instance()->WriteMeshFile(*AssetManager::Instance()->GetMesh("cube"), "C:/Users/fmorris/Documents/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/cube2.mesh");

		AssetManager::Instance()->LoadShader("shader", "C:/Users/fmorris/Documents/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/vertex.vert", "C:/Users/fmorris/Documents/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/fragment.frag");

		while (true) {};
	}
}