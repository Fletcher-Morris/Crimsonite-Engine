#include "AssetManager.h"

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

void AssetManager::LoadMesh(std::string _meshName, std::string _filePath)
{


	std::cout << "Attempting to load mesh : " << _filePath << std::endl;

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
			this->m_loadedMeshNames.push_back(_meshName);
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
		this->m_meshes[_meshName] = GetErrorMesh();
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
	if (!MeshExists(_meshName)) return &m_errorMesh;

	return &m_meshes.at(_meshName);
}

void AssetManager::AddShader(std::string _shaderName)
{
	m_shaders[_shaderName] = Shader();
}

void AssetManager::LoadShader(std::string _shaderName, std::string _filePath)
{
}

void AssetManager::LoadShader(std::string _shaderName, std::string _vertexPath, std::string _fragmentPath)
{
	m_shaders[_shaderName] = Shader(_vertexPath.c_str(), _fragmentPath.c_str());
}

Shader * AssetManager::GetShader(std::string _shaderName)
{
	return &m_shaders.at(_shaderName);
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
	m_errorMesh.indices.push_back(2);
	m_errorMesh.indices.push_back(5);
	m_errorMesh.indices.push_back(0);
	m_errorMesh.indices.push_back(2);
	m_errorMesh.indices.push_back(4);
	m_errorMesh.indices.push_back(0);
	m_errorMesh.indices.push_back(3);
	m_errorMesh.indices.push_back(5);
	m_errorMesh.indices.push_back(0);
	m_errorMesh.indices.push_back(3);
	m_errorMesh.indices.push_back(4);
	m_errorMesh.indices.push_back(1);
	m_errorMesh.indices.push_back(2);
	m_errorMesh.indices.push_back(5);
	m_errorMesh.indices.push_back(1);
	m_errorMesh.indices.push_back(2);
	m_errorMesh.indices.push_back(4);
	m_errorMesh.indices.push_back(1);
	m_errorMesh.indices.push_back(3);
	m_errorMesh.indices.push_back(5);
	m_errorMesh.indices.push_back(1);
	m_errorMesh.indices.push_back(3);
	m_errorMesh.indices.push_back(4);
	m_errorMesh.indices.push_back(4);
	m_errorMesh.indices.push_back(4);
	m_errorMesh.indices.push_back(4);
	m_errorMesh.UploadToGpu();

	m_errorMeshCreated = true;
}

Mesh AssetManager::GetErrorMesh()
{
	CreateErrorMesh();
	return m_errorMesh;
}
