#pragma once

#include <string>

class Asset
{

public:


private:

	//	The location of the file relative to the application
	std::string m_assetLocation;
	//	The name of the asset (shown in-engine)
	std::string m_assetName;
	//	The type of asset, e.g. texture, mesh, shader
	AssetType m_assetType;

};

enum AssetType
{
	Texture,
	Mesh,
	Shader,
	Material,
	Scene
};