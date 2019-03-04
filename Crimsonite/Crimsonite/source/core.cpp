#include "core.h"
#include <stdio.h>
#include <direct.h>

#include "asset/AssetManager.h"
#include "ecs/ECS.h"

namespace Crimson
{
	void Test()
	{
		/*GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::cout << "Failed to initialise GLEW!" << std::endl;
		}
		if (err == GLEW_ERROR_NO_GL_VERSION)
		{
			std::cout << "FAILED TO INITIALISE GLEW MISSING GL VERSION" << std::endl;
		}
		std::cout << "Initialised GLEW (" << glewGetString(GLEW_VERSION) << ")" << std::endl;*/

		printf("CRIMSONITE\n");

		std::cout << _getcwd(NULL, 0) << std::endl;

		AssetManager::Instance()->LoadMesh("cube", "G:/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/cube");
		AssetManager::Instance()->WriteMeshFile(*AssetManager::Instance()->GetMesh("cube"), "G:/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/cube2.mesh");

		//AssetManager::Instance()->LoadShader("shader", "G:/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/vertex.vert", "C:/Users/fmorris/Documents/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/fragment.frag");


		EcsSystem ecs;
		ecs.NewEntity("ENTITY");
		ecs.DestroyEntity("ENTITY");

		while (true) {};
	}
}