#include "core.h"
#include <stdio.h>
#include <direct.h>
#include "asset/AssetManager.h"

namespace Crimson
{
	void Test()
	{
		printf("CRIMSONITE\n");

		std::cout << _getcwd(NULL, 0) << std::endl;

		AssetManager::Instance()->LoadMesh("cube", "G:/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/cube");
		AssetManager::Instance()->WriteMeshFile(*AssetManager::Instance()->GetMesh("cube"), "G:/prco304-final-year-project-Fletcher-Morris/Demo (Output)/Debug/assets/cube2.mesh");

		AssetManager::Instance()->LoadShader("shader", "assets/vertex.vert", "assets/fragment.frag");

		while (true) {};
	}
}