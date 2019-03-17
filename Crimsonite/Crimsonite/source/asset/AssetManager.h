#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

#include "Asset.h"
#include "../mesh/Mesh.h"
#include "../render/Shader.h"
#include "../render/Material.h"

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
	void WriteMeshFile(Mesh _mesh, std::string _filePath);
	Mesh * GetMesh(std::string _meshName);

	//	SHADER METHODS
	void AddShader(std::string _shaderName);
	void LoadShader(std::string _shaderName, std::string _filePath);
	void LoadShader(std::string _shaderName, std::string _vertexPath, std::string _fragmentPath);
	Shader * GetShader(std::string _shaderName);

	//	MATERIAL METHODS
	void AddMaterial(std::string _materialName);
	void AddMaterial(Material _material);
	void LoadMaterial(std::string _filePath);
	Material * GetMaterial(std::string _materialName);

	//	SCENE METHODS

private:

	std::map<std::string, Asset> m_assets;

	//	TEXTURE MAP

	//	MESH MAP
	std::map<std::string, Mesh> m_meshes;
	std::vector<std::string> m_loadedMeshNames;
	bool MeshExists(std::string _meshName);
	int m_latestMeshRevision = 1;
	Mesh m_errorMesh;
	void CreateErrorMesh();
	bool m_errorMeshCreated = false;
	Mesh GetErrorMesh();

	//	SHADER MAP
	std::map<std::string, Shader> m_shaders;

	//	MATERIAL MAP
	std::map<std::string, Material> m_materials;
	int m_latestMaterialRevision = 1;

	//	SCENE MAP

protected:

	//	The static singleton instance of the AssetManager class.
	static AssetManager * m_instance;
};