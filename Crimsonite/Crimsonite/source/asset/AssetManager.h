#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

#include "Asset.h"
#include "../render/Texture.h"
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


	//	Load a Texture from a file with an assigned name.
	void LoadTexture(std::string _textureName, std::string _filePath);
	//	Return a reference to a Texture with a given name.
	Texture * GetTexture(std::string _textureName);


	//	Load a Mesh from a file with an assigned name.
	void LoadMesh(std::string _meshName, std::string _filePath);
	//	Save a given Mesh instance to a file.
	void WriteMeshFile(Mesh _mesh, std::string _filePath);
	//	Return a reference to a Mesh with a given name.
	Mesh * GetMesh(std::string _meshName);


	//	Add a Shader with a given name.
	void AddShader(std::string _shaderName);
	//	Load a Shader from a single file with an assigned name.
	void LoadShader(std::string _shaderName, std::string _filePath);
	//	Load a Shader from a vertex and fragment file with an assigned name.
	void LoadShader(std::string _shaderName, std::string _vertexPath, std::string _fragmentPath);
	//	Return a reference to a Shader with a given name.
	Shader * GetShader(std::string _shaderName);
	//	Forcibly create the defaut Shader.
	void CreateDefaultShader();


	//	Add a Material with a given name.
	void AddMaterial(std::string _materialName);
	//	Add a material to the AssetManager.
	void AddMaterial(Material _material);
	//	Load a Material from a file.
	void LoadMaterial(std::string _filePath);
	//	Return a refernce to a Material with a given name.
	Material * GetMaterial(std::string _materialName);
	//	Forcibly create the default Material.
	void CreateDefaultMaterial();


	//	SCENE METHODS

private:

	std::map<std::string, Asset> m_assets;

	//	The map of loaded textures.
	std::map<std::string, Texture> m_textures;
	//	A vector of the names of all loaded vectors.
	std::vector<std::string> m_loadedTextureNames;
	//	Does a TExture with a given name exist?
	bool TextureExists(std::string _textureName);
	//	Create the error TExture.
	void CreateErrorTexture();
	//	Has the error Texture been created?
	bool m_errorTextureCreated = false;
	//	Return a reference to the error Texture.
	Texture * GetErrorTexture();


	//	The map of loaded meshes.
	std::map<std::string, Mesh> m_meshes;
	//	A vector of the names of all loaded meshes.
	std::vector<std::string> m_loadedMeshNames;
	//	Does a MEsh with a given name exist?
	bool MeshExists(std::string _meshName);
	//	The latest revision of the MESH file format.
	int m_latestMeshRevision = 1;
	//	The error Mesh.
	Mesh m_errorMesh;
	//	Create the error Mesh.
	void CreateErrorMesh();
	//	Has the error mesh beem created?
	bool m_errorMeshCreated = false;
	//	Return a reference to the error Mesh.
	Mesh * GetErrorMesh();


	//	The map of all loaded shaders.
	std::map<std::string, Shader> m_shaders;
	//	A vector of the names of all loaded shaders.
	std::vector<std::string> m_loadedShaderNames;
	//	Does a Shader with a given name exist?
	bool ShaderExists(std::string _shaderName);
	//	Add a Shader to the list of loaded shaders.
	void LoadShaderName(std::string _shaderName);
	//	Has the default Shader been created?
	bool m_defaultShaderCreated = false;
	//	Return a reference to the default Shader.
	Shader * GetDefaultShader();


	//	The map of all loaded materials.
	std::map<std::string, Material> m_materials;
	//	A vector of the names of all loaded materials.
	std::vector<std::string> m_loadedMaterialNames;
	//	Does a Material with a given name exist?
	bool MaterialExists(std::string _materialName);
	//	Add a Material to the list of loaded materials.
	void LoadMaterialName(std::string _materiaName);
	//	The laterst revision of the Material file format.
	int m_latestMaterialRevision = 1;
	//	Has the default Material been created?
	bool m_defaultMaterialCreated = false;
	//	Return a reference to the default Material.
	Material * GetDefaultMaterial();


	//	SCENE MAP

protected:

	//	The static singleton instance of the AssetManager class.
	static AssetManager * m_instance;
};