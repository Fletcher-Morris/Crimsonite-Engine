#include "AssetManager.h"
#include <SOIL2/SOIL2.h>

AssetManager * AssetManager::m_instance(0);
AssetManager * AssetManager::Instance()
{
	if (!m_instance) m_instance = new AssetManager();
	return m_instance;
}

AssetManager::~AssetManager()
{
	m_instance = 0;
}

void AssetManager::LoadTexture(std::string _textureName, std::string _filePath)
{
	int width;
	int height;
	unsigned char* data = SOIL_load_image(_filePath.c_str(), &width, &height, NULL, SOIL_LOAD_RGBA);

	if (data)
	{
		m_textures[_textureName] = Texture(width, height, data);
		Texture * tex = &m_textures.at(_textureName);

		glGenTextures(1, &tex->TextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->GetWidth(), tex->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->ImageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		std::cout << "Loaded Texture '" << _textureName << "' from : " << _filePath << std::endl;
	}
	else
	{
		std::cout << "Failed To Load Texture '" << _textureName << "' from : " << _filePath << "!" << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(data);
}

void AssetManager::LoadMesh(std::string _meshName, std::string _filePath)
{
	//	Prepare the vertex vectors.
	std::vector<glm::vec3>		input_positions;
	std::vector<glm::vec3>		input_normals;
	std::vector<glm::vec3>		input_uvs;
	std::vector<IndexTriplet>	input_triplets;
	std::vector<Vertex>			output_vertices;
	std::vector<unsigned int>	output_indices;

	struct stat statFile;
	//	Check if a MESH version of the file allready exists.
	if (stat((_filePath + ".mesh").c_str(), &statFile) == 0)
	{
		std::cout << "Loading MESH file : " << _filePath << std::endl;
		//	Proccess the mesh file.

//	Temporary error-suppressor.
#pragma warning(push)
#pragma warning(disable: 4996)
		FILE * file = fopen((_filePath + ".mesh").c_str(), "r");

		int res = 0;
		//	Read though the mesh file.
		while (res != EOF)
		{
			char lineHeader[128];
			res = fscanf(file, "%s", lineHeader);
			//	Check for revision header.
			if (strcmp(lineHeader, "r") == 0)
			{
				int fileRevision;
				fscanf(file, "%d", &fileRevision);
				//	Check if the mesh file matches the latest format revision.
				if (fileRevision != m_latestMeshRevision)
				{
					std::cout << "Mesh : " + _filePath + " does not match the latest mesh format revision (" << m_latestMeshRevision << ")!" << std::endl;
				}

			}
			//	Check for vertex header.
			else if (strcmp(lineHeader, "v") == 0)
			{
				Vertex newVertex;
				glm::vec3 newPosition;
				glm::vec3 newNormal;
				glm::vec2 newUv;
				fscanf(file, "%f %f %f %f %f %f %f %f", &newPosition.x, &newPosition.y, &newPosition.z, &newNormal.x, &newNormal.y, &newNormal.z, &newUv.x, &newUv.y);
				newVertex.position = newPosition;
				newVertex.normal = newNormal;
				newVertex.uv = newUv;
				output_vertices.push_back(newVertex);
			}
			//	Check for index header.
			else if (strcmp(lineHeader, "i") == 0)
			{
				int newIndex;
				fscanf(file, "%d", &newIndex);
				output_indices.push_back(newIndex);
			}			
		}
		//	Create the new mesh.
		Mesh newMesh(output_vertices, output_indices);
		//	Upload the new mesh to the GPU.
		newMesh.UploadToGpu();
		//	Store the new mesh in the asset manager.
		this->m_meshes[_meshName] = newMesh;
		//	Add this mesh's name to the list of loaded mesh names.
		if (std::find(m_loadedMeshNames.begin(), m_loadedMeshNames.end(), _meshName) == m_loadedMeshNames.end())
		{
			m_loadedMeshNames.push_back(_meshName);
		}

		std::cout << "Loaded MESH file : " << _filePath << std::endl;
//	End error-suppressor.
#pragma warning(pop)
	}
	else if (stat((_filePath + ".obj").c_str(), &statFile) == 0)
	{
		//	Load the basic OBJ version.
		std::cout << "Loading OBJ file : " << _filePath << std::endl;
	}
	else if (stat((_filePath).c_str(), &statFile) == 0)
	{
		//	Load the basic OBJ version.
		std::cout << "Loading MISC file : " << _filePath << std::endl;
	}
	else
	{
		std::cout << "Could not locate mesh file at : " << _filePath << std::endl;
	}
}

void AssetManager::WriteMeshFile(Mesh _mesh, std::string _filePath)
{
	//	Create the new file.
	std::ofstream file(_filePath);
	file << "# Crimsonite MESH File" << std::endl;

	//	Add the timestamp.
//	Temporary error-suppressor.
#pragma warning(push)
#pragma warning(disable: 4996)
	time_t t = time(NULL);
	tm* timePtr = localtime(&t);
	file << "# Created: " << (timePtr->tm_mday) << "/" << (timePtr->tm_mon) + 1 << "/" << (timePtr->tm_year) + 1900;
	file << " " << (timePtr->tm_hour) << ":" << (timePtr->tm_min) << ":" << (timePtr->tm_sec) << std::endl;
//	End error-suppressor.
#pragma warning(pop)

	std::string fileData;

	//	Add the mesh revision.
	fileData.append("r ");
	fileData.append(std::to_string(m_latestMeshRevision));
	fileData.append("\n\n");

	//	Prepare the mesh vertex data.
	int v = 0;
	while (v < _mesh.vertices.size())
	{
		fileData.append("v ");
		fileData.append(std::to_string(_mesh.vertices[v].position.x) + " ");
		fileData.append(std::to_string(_mesh.vertices[v].position.y) + " ");
		fileData.append(std::to_string(_mesh.vertices[v].position.z) + " ");
		fileData.append(std::to_string(_mesh.vertices[v].normal.x) + " ");
		fileData.append(std::to_string(_mesh.vertices[v].normal.y) + " ");
		fileData.append(std::to_string(_mesh.vertices[v].normal.z) + " ");
		fileData.append(std::to_string(_mesh.vertices[v].uv.x) + " ");
		fileData.append(std::to_string(_mesh.vertices[v].uv.y));
		fileData.append("\n");
		v++;
	}

	//	Prepare the mesh index data.
	int i = 0;
	while (i < _mesh.indices.size())
	{
		fileData += "\ni ";
		fileData.append(std::to_string(_mesh.indices[i]));
		i++;
	}

	//	Add the vertex count.
	file << "# " << v << " vertices" << std::endl;
	//	Add the index count.
	file << "# " << i << " indices" << std::endl;
	file << std::endl;
	//	Add the vertex and index data.
	file << fileData;

	//	Save and close the file.
	file.close();

	std::cout << "Saved mesh file : " << _filePath << std::endl;
}

Mesh * AssetManager::GetMesh(std::string _meshName)
{
	if (!MeshExists(_meshName))
	{
		std::cout << "Mesh '" << _meshName << "' does not exist yet, but something is trying to access it." << std::endl;
		return GetErrorMesh();
	}
	return &m_meshes.at(_meshName);
}

void AssetManager::AddShader(std::string _shaderName)
{
	m_shaders[_shaderName] = Shader();
	LoadShaderName(_shaderName);
}

void AssetManager::LoadShader(std::string _shaderName, std::string _filePath)
{
}

void AssetManager::LoadShader(std::string _shaderName, std::string _vertexPath, std::string _fragmentPath)
{
	m_shaders[_shaderName] = Shader(_vertexPath.c_str(), _fragmentPath.c_str());
	LoadShaderName(_shaderName);
}

Shader * AssetManager::GetShader(std::string _shaderName)
{
	if (!ShaderExists(_shaderName))
	{
		std::cout << "Shader '" << _shaderName << "' does not exist yet, but something is trying to access it." << std::endl;
		return GetDefaultShader();
	}
	return &m_shaders.at(_shaderName);
}

void AssetManager::AddMaterial(std::string _materialName)
{
	m_materials[_materialName] = Material(_materialName);
	LoadMaterialName(_materialName);
}

void AssetManager::AddMaterial(Material _material)
{
	m_materials[_material.GetName()] = _material;
	LoadMaterialName(_material.GetName());
}

void AssetManager::LoadMaterial(std::string _filePath)
{
	Material newMat = Material();

	struct stat statFile;
	//	Check if a file allready exists.
	if (stat((_filePath + ".material").c_str(), &statFile) == 0)
	{

//	Temporary error-suppressor.
#pragma warning(push)
#pragma warning(disable: 4996)
		FILE * file = fopen((_filePath + ".material").c_str(), "r");

		int res = 0;
		//	Read though the mesh file.
		while (res != EOF)
		{
			char lineHeader[128];
			res = fscanf(file, "%s", lineHeader);
			//	Check for revision header.
			if (strcmp(lineHeader, "r") == 0)
			{
				int fileRevision;
				fscanf(file, "%d", &fileRevision);
				//	Check if the mesh file matches the latest format revision.
				if (fileRevision != m_latestMaterialRevision)
				{
					std::cout << "Material : " + _filePath + " does not match the latest material format revision (" << m_latestMaterialRevision << ")!" << std::endl;
				}

			}
			//	Get the material name.
			else if (strcmp(lineHeader, "name") == 0)
			{
				std::string name;
				fscanf(file, "%s", &lineHeader);
				name = (std::string)lineHeader;
				if (name != "") newMat.SetName(name);
			}
			//	Get the shader type.
			else if (strcmp(lineHeader, "shader") == 0)
			{
				std::string shaderName;
				fscanf(file, "%s", &lineHeader);
				shaderName = (std::string)lineHeader;
				if (shaderName != "") newMat.SetShader(GetShader(shaderName));
			}
			//	Get the shader type.
			else if (strcmp(lineHeader, "color") == 0)
			{
				glm::vec3 color;
				fscanf(file, "%f,%f,%f", &color.x, &color.y, &color.z);
				newMat.SetColor(color);
			}
		}

		std::cout << "Loaded Material file from : " << _filePath << std::endl;
		//	End error-suppressor.
#pragma warning(pop)
	}
	AddMaterial(newMat);
}

Material * AssetManager::GetMaterial(std::string _materialName)
{
	if (!MaterialExists(_materialName))
	{
		std::cout << "Material '" << _materialName << "' does not exist yet, but something is trying to access it." << std::endl;
		return GetDefaultMaterial();
	}
	return &m_materials.at(_materialName);
}

bool AssetManager::MeshExists(std::string _meshName)
{
	for (int i = 0; i < m_loadedMeshNames.size(); i++)
	{
		if (m_loadedMeshNames[i] == _meshName) return true;
	}
	return false;
}

void AssetManager::CreateErrorMesh()
{
	if (m_errorMeshCreated) return;
	Vertex v;
	v.position = { 0.0, 0.5, 0.0 };
	v.normal = { 0.0, 0.0, 0.0 };
	v.uv = { 0.0, 1.0};
	m_errorMesh.vertices.push_back(v);
	v.position = { 0.0, -0.5, 0.0 };
	v.normal = { 0.0, 0.0, 0.0 };
	v.uv = { 0.0, 1.0 };
	m_errorMesh.vertices.push_back(v);
	v.position = { 0.5, 0.0, 0.0 };
	v.normal = { 0.0, 0.0, 0.0 };
	v.uv = { 0.0, 1.0 };
	m_errorMesh.vertices.push_back(v);
	v.position = { -0.5, 0.0, 0.0 };
	v.normal = { 0.0, 0.0, 0.0 };
	v.uv = { 0.0, 1.0 };
	m_errorMesh.vertices.push_back(v);
	v.position = { 0.0, 0.0, 0.5 };
	v.normal = { 0.0, 0.0, 0.0 };
	v.uv = { 0.0, 1.0 };
	m_errorMesh.vertices.push_back(v);
	v.position = { 0.0, 0.0, -0.5 };
	v.normal = { 0.0, 0.0, 0.0 };
	v.uv = { 0.0, 1.0 };
	m_errorMesh.vertices.push_back(v);
	m_errorMesh.indices.push_back(0);//top
	m_errorMesh.indices.push_back(2);//right
	m_errorMesh.indices.push_back(4);//front
	m_errorMesh.indices.push_back(0);//top
	m_errorMesh.indices.push_back(4);//front
	m_errorMesh.indices.push_back(3);//left
	m_errorMesh.indices.push_back(0);//top
	m_errorMesh.indices.push_back(5);//back
	m_errorMesh.indices.push_back(2);//right
	m_errorMesh.indices.push_back(0);//top
	m_errorMesh.indices.push_back(3);//left
	m_errorMesh.indices.push_back(5);//back
	m_errorMesh.indices.push_back(1);//bottom
	m_errorMesh.indices.push_back(4);//front
	m_errorMesh.indices.push_back(2);//right
	m_errorMesh.indices.push_back(1);//bottom
	m_errorMesh.indices.push_back(3);//left
	m_errorMesh.indices.push_back(4);//front
	m_errorMesh.indices.push_back(1);//bottom
	m_errorMesh.indices.push_back(2);//back
	m_errorMesh.indices.push_back(5);//right
	m_errorMesh.indices.push_back(1);//bottom
	m_errorMesh.indices.push_back(5);//back
	m_errorMesh.indices.push_back(3);//left
	m_errorMesh.UploadToGpu();
	m_errorMeshCreated = true;
	std::cout << "Created Error Mesh." << std::endl;
}

Mesh * AssetManager::GetErrorMesh()
{
	CreateErrorMesh();
	return &m_errorMesh;
}

bool AssetManager::ShaderExists(std::string _shaderName)
{
	for (int i = 0; i < m_loadedShaderNames.size(); i++)
	{
		if (m_loadedShaderNames[i] == _shaderName) return true;
	}
	return false;
}

void AssetManager::LoadShaderName(std::string _shaderName)
{
	if (std::find(m_loadedShaderNames.begin(), m_loadedShaderNames.end(), _shaderName) == m_loadedShaderNames.end())
	{
		m_loadedShaderNames.push_back(_shaderName);
	}
}

void AssetManager::CreateDefaultShader()
{
	if (m_defaultShaderCreated == true) return;
	std::string vertexString =
		"#version 330 core\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"layout(location = 2) in vec2 texCoord;\n"
		"out vec2 v_TexCoord;\n"
		"uniform mat4 u_MVP;\n"
		"void main()\n"
		"{\n"
		"\tgl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0);\n"
		"\tv_TexCoord = texCoord;\n"
		"}";
	std::string fragmentString =
		"#version 330 core\n"
		"layout(location = 0) out vec4 color;\n"
		"in vec2 v_TexCoord;\n"
		"uniform vec4 u_Color;\n"
		"uniform sampler2D u_Texture;\n"
		"vec3 colour;\n"
		"void main()\n"
		"{\n"
		"\tvec4 texColor = texture(u_Texture, v_TexCoord);\n"
		"\tcolor = texColor * u_Color;\n"
		"}";

	Shader shader = Shader();
	shader.Compile(vertexString, fragmentString);
	m_shaders["default"] = shader;
	m_defaultShaderCreated = true;
	LoadShaderName("default");
	std::cout << "Created Default Shader." << std::endl;
}

Shader * AssetManager::GetDefaultShader()
{
	CreateDefaultShader();
	return &m_shaders.at("default");
}

bool AssetManager::MaterialExists(std::string _materialName)
{
	for (int i = 0; i < m_loadedMaterialNames.size(); i++)
	{
		if (m_loadedMaterialNames[i] == _materialName) return true;
	}
	return false;
}

void AssetManager::LoadMaterialName(std::string _materiaName)
{
	if (std::find(m_loadedMaterialNames.begin(), m_loadedMaterialNames.end(), _materiaName) == m_loadedMaterialNames.end())
	{
		m_loadedMaterialNames.push_back(_materiaName);
	}
}

void AssetManager::CreateDefaultMaterial()
{
	if (m_defaultMaterialCreated == true) return;
	Material newMat = Material("default");
	newMat.SetShader("default");
	newMat.SetColor({ 0.5f,0.5f,0.5f });
	m_materials["default"] = newMat;
	m_defaultMaterialCreated = true;
	std::cout << "Created Default Material." << std::endl;
}

Material * AssetManager::GetDefaultMaterial()
{
	CreateDefaultMaterial();
	return &m_materials.at("default");
}
