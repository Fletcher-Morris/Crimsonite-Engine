#pragma once

#include <map>
#include <vector>

#include "Asset.h"
#include "../mesh/Mesh.h"

class AssetManager
{

public:

	//	For accessing the singleton instance of the AssetManager class.
	static AssetManager* Instance();
	//	Virtual destructor for the asset manager class.
	virtual ~AssetManager();


	//	TEXTURE METHODS

	//	MESH METHODS
	void LoadMesh(std::string _meshName, std::string _filePath);
	Mesh * GetMesh(std::string _meshName);

	//	SHADER METHODS

	//	MATERIAL METHODS

	//	SCENE METHODS

private:

	std::map<std::string, Asset> m_assets;

	//	TEXTURE MAP

	//	MESH MAP
	std::vector<std::string> m_loadedMeshNames;

	//	SHADER MAP

	//	MATERIAL MAP

	//	SCENE MAP

protected:

	//	The static singleton instance of the AssetManager class.
	static AssetManager * m_instance;
};