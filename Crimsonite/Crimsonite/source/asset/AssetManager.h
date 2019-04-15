#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

#include "Asset.h"
#include "../render/Texture.h"
#include "../render/FrameBuffer.h"
#include "../mesh/Mesh.h"
#include "../render/Shader.h"
#include "../render/Material.h"
#include "Scene.h"

class CrimsonCore;

class AssetManager
{

public:

	//	For accessing the singleton instance of the AssetManager class.
	static AssetManager* Instance();
	//	Virtual destructor for the asset manager class.
	virtual ~AssetManager();

	static void SetEngine(CrimsonCore * _core);

	//	Load a Texture from a file with an assigned name.
	static void LoadTexture(std::string _textureName, std::string _filePath);
	//	Create a blank Texture.
	static void CreateTexture(std::string _textureName, int _width, int _height);
	static void CreateTexture(std::string _textureName);
	static void CreateTexture(std::string _textureName, int _width, int _height, int _glId);
	//	Return a reference to a Texture with a given name.
	static Texture * GetTexture(std::string _textureName);
	static Texture * GetTexture(int _textureId) { return &Instance()->m_textures.at(Instance()->m_loadedTextureNames[_textureId]); }
	static int TextureCount() { return Instance()->m_loadedTextureNames.size(); }


	//	Create a new FrameBuffer.
	static void CreateFrameBuffer(std::string _bufferName, int _width, int _height);
	static void CreateFrameBuffer(std::string _bufferName, int _width, int _height, bool _useDepth);
	//	Bind a specified FrameBuffer.
	static void BindFrameBuffer(std::string _bufferName);
	//	Return a specific FrameBuffer.
	static FrameBuffer * GetFrameBuffer(std::string _bufferName);


	//	Load a Mesh from a file with an assigned name.
	static void LoadMesh(std::string _meshName, std::string _filePath);
	//	Save a given Mesh instance to a file.
	static void WriteMeshFile(Mesh _mesh, std::string _filePath);
	//	Return a reference to a Mesh with a given name.
	static Mesh * GetMesh(std::string _meshName);
	static Mesh * GetMesh(int _meshId) { return &Instance()->m_meshes.at(Instance()->m_loadedMeshNames[_meshId]); }
	static std::string GetMeshName(int _meshId) { return Instance()->m_loadedMeshNames[_meshId]; }
	static int MeshCount() { return Instance()->m_loadedMeshNames.size(); }


	//	Add a Shader with a given name.
	static void AddShader(std::string _shaderName);
	//	Load a Shader from a single file with an assigned name.
	static void LoadShader(std::string _shaderName, std::string _filePath);
	//	Load a Shader from a vertex and fragment file with an assigned name.
	static void LoadShader(std::string _shaderName, std::string _vertexPath, std::string _fragmentPath);
	//	Return a reference to a Shader with a given name.
	static Shader * GetShader(std::string _shaderName);
	//	Forcibly create the defaut Shader.
	static void CreateDefaultShader();
	//	Create the passthrough Shader.
	static void CreatePassthroughShader();
	//	Return a reference to the passthrough Shader.
	static 	Shader * GetPassthroughShader();


	//	Add a Material with a given name.
	static void AddMaterial(std::string _materialName);
	//	Add a material to the AssetManager.
	static void AddMaterial(Material _material);
	//	Load a Material from a file.
	static void LoadMaterial(std::string _filePath);
	//	Return a refernce to a Material with a given name.
	static Material * GetMaterial(std::string _materialName);
	//	Forcibly create the default Material.
	static void CreateDefaultMaterial();
	//	Return a reference to the default Material.
	static Material * GetDefaultMaterial();


	//	SCENE METHODS
	static void LoadScene(std::string _scenePath);
	static Scene * GetScene(std::string _sceneName);
	static Scene * GetScene(int _sceneId);
	static void SaveScene(Scene * _scene);
	static void SaveScene(std::string _sceneName);
	static void OpenScene(Scene * _scene);
	static void OpenScene(std::string _sceneName);
	static void OpenScene(int _sceneId);
	static void ChangeLoadedSceneName(std::string _currentName, std::string _newName);

private:

	CrimsonCore * m_core;

	std::map<std::string, Asset> m_assets;

	//	The map of loaded textures.
	std::map<std::string, Texture> m_textures;
	//	A vector of the names of all loaded vectors.
	std::vector<std::string> m_loadedTextureNames;
	//	Does a Texture with a given name exist?
	bool TextureExists(std::string _textureName);
	//	Check if a unique vertex already exists.
	int FindExistingVertex(std::vector<Vertex> _inputVector, glm::vec3 _position, glm::vec2 _uv, glm::vec3 _normal);
	//	Create the error Texture.
	void CreateErrorTexture();
	//	Has the error Texture been created?
	bool m_errorTextureCreated = false;
	//	Return a reference to the error Texture.
	Texture * GetErrorTexture();

	//	The map of framebuffers.
	std::map<std::string, FrameBuffer> m_frameBuffers;
	//	A vector of the names of all loaded FrameBuffers.
	std::vector<std::string> m_loadedFrameBufferNames;
	//	Does a FrameBuffer with a given name exist?
	bool FrameBufferExists(std::string _bufferName);


	//	The map of loaded meshes.
	std::map<std::string, Mesh> m_meshes;
	//	A vector of the names of all loaded meshes.
	std::vector<std::string> m_loadedMeshNames;
	//	Add a Mesh to the list of loaded meshes.
	void LoadMeshName(std::string _meshName);
	//	Does a Mesh with a given name exist?
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
	//	Has the passthrough Shader been created?
	bool m_passthroughShaderCreated = false;


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


	//	The map of all loaded scenes.
	std::map<int, Scene> m_scenes;
	//	A vector of all loaded scene names.
	std::vector<std::string> m_loadedSceneNames;
	int m_sceneCount;
	//	Add a Scene to the list of loaded scenes.
	void LoadSceneName(std::string _sceneName);
	//	Does a scene with a given name exist?
	bool SceneExists(std::string _sceneName);

protected:

	//	The static singleton instance of the AssetManager class.
	static AssetManager * m_instance;
};