#pragma once

#include <map>
#include <vector>

class AssetManager
{

public:

	//	For accessing the singleton instance of the AssetManager class.
	static AssetManager* Instance();
	//	Virtual destructor for the asset manager class.
	virtual ~AssetManager();


	//	TEXTURE METHODS
	//	MESH METHODS
	//	SHADER METHODS
	//	MATERIAL METHODS
	//	SCENE METHODS

private:

	//	TEXTURE MAP
	//	MESH MAP
	//	SHADER MAP
	//	MATERIAL MAP
	//	SCENE MAP

protected:

	//	The static singleton instance of the AssetManager class.
	static AssetManager * m_instance;
};