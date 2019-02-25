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
	std::vector<glm::vec3>		input_positions;
	std::vector<glm::vec3>		input_normals;
	std::vector<glm::vec3>		input_uvs;
	std::vector<IndexTriplet>	input_triplets;
	std::vector<Vertex>			output_vertices;
	std::vector<unsigned int>	output_indices;

	struct stat statFile;
	//	Check is a mesh version of the file exists.
	if (stat((_filePath + ".mesh").c_str(), &statFile) == 0)
	{
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
		}
	}
}

Mesh * AssetManager::GetMesh(std::string _meshName)
{
	return nullptr;
}

void AssetManager::WriteMeshFile(Mesh _mesh, std::string _filePath)
{
}
