#include "core.h"
#include <stdio.h>
#include "asset/AssetManager.h"

namespace Crimson
{
	void Test()
	{
		printf("CRIMSONITE\n");

		AssetManager::Instance()->LoadMesh("cube", "assets/cube");
		AssetManager::Instance()->WriteMeshFile(*AssetManager::Instance()->GetMesh("cube"), "assets/cube2.mesh");

		while (true) {};
	}
}