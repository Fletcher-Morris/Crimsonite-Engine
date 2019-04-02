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
		m_textures[_textureName] = Texture(_textureName, width, height, data);
		Texture * tex = &m_textures.at(_textureName);

		glGenTextures(1, &tex->TextureId);
		glBindTexture(GL_TEXTURE_2D, tex->TextureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex->GetWidth(), tex->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->ImageData);
		glBindTexture(GL_TEXTURE_2D, 0);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (!TextureExists(_textureName))
		{
			m_loadedTextureNames.push_back(_textureName);
		}

		std::cout << "Loaded Texture '" << _textureName << "'."<< std::endl;

		SOIL_free_image_data(data);
	}
	else
	{
		std::cout << "Failed To Load Texture '" << _textureName << "' from : " << _filePath << "!" << std::endl;
	}
}

void AssetManager::CreateTexture(std::string _textureName)
{
	CreateTexture(_textureName, 512, 512);
}

void AssetManager::CreateTexture(std::string _textureName, int _width, int _height, int _glId)
{
	m_textures[_textureName] = Texture(_textureName, _width, _height);
	Texture * tex = &m_textures.at(_textureName);
	tex->TextureId = _glId;
	if (!TextureExists(_textureName))
	{
		m_loadedTextureNames.push_back(_textureName);
	}
	std::cout << "Created Texture '" << _textureName << "'." << std::endl;
}

void AssetManager::CreateTexture(std::string _textureName, int _width, int _height)
{
	m_textures[_textureName] = Texture(_textureName, _width, _height);
	Texture * tex = &m_textures.at(_textureName);
	glGenTextures(1, &tex->TextureId);
	glBindTexture(GL_TEXTURE_2D, tex->TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (!TextureExists(_textureName))
	{
		m_loadedTextureNames.push_back(_textureName);
	}

	std::cout << "Created Texture '" << _textureName << "'." << std::endl;

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture * AssetManager::GetTexture(std::string _textureName)
{
	if (!TextureExists(_textureName))
	{
		std::cout << "Texture '" << _textureName << "' does not exist yet, but something is trying to access it." << std::endl;
		return GetErrorTexture();
	}
	return &m_textures.at(_textureName);
}

void AssetManager::CreateFrameBuffer(std::string _bufferName, int _width, int _height)
{
	CreateFrameBuffer(_bufferName, _width, _height, true);
}

void AssetManager::CreateFrameBuffer(std::string _bufferName, int _width, int _height, bool _useDepth)
{
	m_frameBuffers[_bufferName] = FrameBuffer();
	FrameBuffer * buffer = &m_frameBuffers.at(_bufferName);

	glGenFramebuffers(1, &buffer->FrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer->FrameBufferId);

	CreateTexture(_bufferName, _width, _height);
	buffer->SetTexture(GetTexture(_bufferName));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures.at(_bufferName).TextureId, 0);

	glGenRenderbuffers(1, &buffer->DepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer->DepthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->DepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR CREATEING FRAME BUFFER" << std::endl;
	}
	else
	{

		if (!FrameBufferExists(_bufferName))
		{
			m_loadedFrameBufferNames.push_back(_bufferName);
		}

		std::cout << "Created Frame Buffer '" << _bufferName << "'." << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AssetManager::BindFrameBuffer(std::string _bufferName)
{
	if (FrameBufferExists(_bufferName))
	GetFrameBuffer(_bufferName)->Bind();
}

FrameBuffer * AssetManager::GetFrameBuffer(std::string _bufferName)
{
	if (!FrameBufferExists(_bufferName))
	{
		std::cout << "FrameBuffer '" << _bufferName << "' does not exist yet, but something is trying to access it." << std::endl;
	}
	return &m_frameBuffers.at(_bufferName);
}

void AssetManager::LoadMesh(std::string _meshName, std::string _filePath)
{
	//	Temporary error-suppressor.
#pragma warning(push)
#pragma warning(disable: 4996)


	//	Prepare the vertex vectors.
	std::vector<glm::vec3>		input_positions;
	std::vector<glm::vec3>		input_normals;
	std::vector<glm::vec2>		input_uvs;
	std::vector<IndexTriplet>	input_triplets;
	std::vector<Vertex>			output_vertices;
	std::vector<unsigned int>	output_indices;

	struct stat statFile;
	//	Check if a MESH version of the file allready exists.
	if (stat((_filePath + ".mesh").c_str(), &statFile) == 0)
	{
		FILE * file = fopen((_filePath + ".mesh").c_str(), "r");

		int res = 0;
		//	Read though the mesh file.
		while (res != EOF)
		{
			char lineHeader[32];
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
		LoadMeshName(_meshName);

		std::cout << "Loaded MESH file : " << _filePath << std::endl;
	}
	else if (stat((_filePath + ".obj").c_str(), &statFile) == 0)
	{
		//	Load the basic OBJ version.

		FILE * file = fopen((_filePath + ".obj").c_str(), "r");
		if (file == NULL) {}
		else
		{
			int res = 0;
			while (res != EOF)
			{
				char lineHeader[32];
				res = fscanf(file, "%s", lineHeader);
				Vertex temp_vertex;
				if (strcmp(lineHeader, "v") == 0)
				{
					glm::vec3 position;
					fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
					input_positions.push_back(position);
				}
				else if (strcmp(lineHeader, "vt") == 0)
				{
					glm::vec2 uv;
					fscanf(file, "%f %f\n", &uv.x, &uv.y);
					input_uvs.push_back(uv);
				}
				else if (strcmp(lineHeader, "vn") == 0)
				{
					glm::vec3 normal;
					fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					input_normals.push_back(normal);
				}
				else if (strcmp(lineHeader, "f") == 0)
				{
					bool match = false;
					IndexTriplet trip[3];
					int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &trip[0].position, &trip[0].uv, &trip[0].normal,
						&trip[1].position, &trip[1].uv, &trip[1].normal, &trip[2].position, &trip[2].uv, &trip[2].normal);
					if (matches != 9)
					{
						matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &trip[0].position, &trip[0].uv, &trip[0].normal,
							&trip[1].position, &trip[1].uv, &trip[1].normal, &trip[2].position, &trip[2].uv, &trip[2].normal);
						if (matches != 9) {}
						else { match = true; }
					}
					else { match = true; }
					if (match == true)
					{
						for (int i = 0; i < 3; i++)
						{
							trip[i].position--;
							trip[i].uv--;
							trip[i].normal--;
							input_triplets.push_back(trip[i]);
						}
					}
					else
					{
						std::cout << "Error parsing obj file: " << _filePath << std::endl;
					}
				}
			}
			for (int i = 0; i < input_triplets.size(); i++)
			{
				glm::vec3 tempPos = input_positions[input_triplets[i].position];
				glm::vec2 tempUv = input_uvs[input_triplets[i].uv];
				glm::vec3 tempNorm = input_normals[input_triplets[i].normal];
				int ind = FindExistingVertex(output_vertices, tempPos, tempUv, tempNorm);
				if (ind == -1)
				{
					Vertex newVert;
					newVert.position = tempPos;
					newVert.uv = tempUv;
					newVert.normal = tempNorm;

					output_vertices.push_back(newVert);
					ind = output_vertices.size() - 1;
				}
				output_indices.push_back(ind);
			}
			m_meshes[_meshName] = Mesh(output_vertices, output_indices);
			WriteMeshFile(m_meshes[_meshName], _filePath + ".mesh");
			LoadMeshName(_meshName);
			std::cout << "Loaded OBJ file : " << _filePath << std::endl;
		}
	}
	else
	{
		std::cout << "Could not locate 3D model file at : " << _filePath << std::endl;
	}

	//	End error-suppressor.
#pragma warning(pop)
}
int AssetManager::FindExistingVertex(std::vector<Vertex> _inputVector, glm::vec3 _position, glm::vec2 _uv, glm::vec3 _normal)
{
	for (int i = 0; i < _inputVector.size(); i++)
	{
		if (_inputVector[i].position == _position
			&& _inputVector[i].normal == _normal
			&& _inputVector[i].uv == _uv) return i;
	}
	return -1;
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
	m_shaders[_shaderName] = Shader(_shaderName, _vertexPath.c_str(), _fragmentPath.c_str());
	LoadShaderName(_shaderName);
}

Shader * AssetManager::GetShader(std::string _shaderName)
{
	if (_shaderName == "default")
	{
		return GetDefaultShader();
	}
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

		int propertyRefCount = 0;

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
					std::cout << "Material is out of date (" << _filePath << ")" << std::endl;
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
			//	Get a Texture reference.
			else if (strcmp(lineHeader, "texture-main") == 0)
			{
				std::string textureName;
				fscanf(file, "%s", &lineHeader);
				textureName = (std::string)lineHeader;
				newMat.ReservePropertyName("MainTex");
				newMat.SetTextureProperty("MainTex", textureName);
				propertyRefCount++;
			}
			//	Get a Texture property.
			else if (strcmp(lineHeader, "texture-property") == 0)
			{
				std::string propertyName;
				fscanf(file, "%s", &lineHeader);
				propertyName = (std::string)lineHeader;
				newMat.ReservePropertyName(propertyName);
			}
			//	Get a Texture reference.
			else if (strcmp(lineHeader, "texture-reference") == 0)
			{
				std::string textureName;
				fscanf(file, "%s", &lineHeader);
				textureName = (std::string)lineHeader;
				newMat.SetTextureProperty(newMat.GetReservedPropertyName(propertyRefCount), textureName);
				propertyRefCount++;
			}
		}

		std::cout << "Loaded Material '" << newMat.GetName() << "'." << std::endl;
		//	End error-suppressor.
#pragma warning(pop)
	}
	AddMaterial(newMat);
}

Material * AssetManager::GetMaterial(std::string _materialName)
{
	if (_materialName == "default")
	{
		return GetDefaultMaterial();
	}
	if (!MaterialExists(_materialName))
	{
		std::cout << "Material '" << _materialName << "' does not exist yet, but something is trying to access it." << std::endl;
		return GetDefaultMaterial();
	}
	return &m_materials.at(_materialName);
}

void AssetManager::LoadMeshName(std::string _meshName)
{
	if (std::find(m_loadedMeshNames.begin(), m_loadedMeshNames.end(), _meshName) == m_loadedMeshNames.end())
	{
		m_loadedMeshNames.push_back(_meshName);
	}
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
		"out vec2 TexCoord;\n"
		"uniform mat4 u_MVP;\n"
		"void main()\n"
		"{\n"
		"\tgl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0);\n"
		"\tTexCoord = texCoord;\n"
		"}";
	std::string fragmentString =
		"#version 330 core\n"
		"layout(location = 0) out vec4 color;\n"
		"in vec2 TexCoord;\n"
		"uniform vec4 Color;\n"
		"uniform sampler2D MainTex;\n"
		"void main()\n"
		"{\n"
		"\tvec4 texColor = texture(MainTex, TexCoord);\n"
		"\tcolor = texColor * Color;\n"
		"}";

	Shader shader = Shader("default");
	shader.Compile(vertexString, fragmentString);
	m_shaders["default"] = shader;
	m_defaultShaderCreated = true;
	LoadShaderName("default");
	std::cout << "Created Default Shader." << std::endl;
}

void AssetManager::CreatePassthroughShader()
{
	if (m_passthroughShaderCreated == true) return;
	std::string vertexString =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"layout(location = 2) in vec2 texCoord;\n"
		"\n"
		"out vec2 TexCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(position.x, position.y, 0.0, 1.0); \n"
		"    TexCoord = texCoord;\n"
		"} ";
	std::string fragmentString =
		"#version 330 core\n"
		"layout(location = 0) out vec4 color;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D MainTex;\n"
		"void main()\n"
		"{\n"
		"color = texture(MainTex, TexCoord);\n"
		"}";

	Shader shader = Shader();
	shader.Compile(vertexString, fragmentString);
	m_shaders["passthrough"] = shader;
	m_passthroughShaderCreated = true;
	LoadShaderName("passthrough");
	std::cout << "Created Passthrough Shader." << std::endl;
}

Shader * AssetManager::GetDefaultShader()
{
	CreateDefaultShader();
	return &m_shaders.at("default");
}

Shader * AssetManager::GetPassthroughShader()
{
	CreatePassthroughShader();
	return &m_shaders.at("passthrough");
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
	newMat.SetColor({ 1.0f,1.0f,1.0f });
	m_materials["default"] = newMat;
	m_defaultMaterialCreated = true;
	std::cout << "Created Default Material." << std::endl;
}

bool AssetManager::TextureExists(std::string _textureName)
{
	for (int i = 0; i < m_loadedTextureNames.size(); i++)
	{
		if (m_loadedTextureNames[i] == _textureName) return true;
	}
	return false;
}

void AssetManager::CreateErrorTexture()
{
}

Texture * AssetManager::GetErrorTexture()
{
	return nullptr;
}

bool AssetManager::FrameBufferExists(std::string _bufferName)
{
	for (int i = 0; i < m_loadedFrameBufferNames.size(); i++)
	{
		if (m_loadedFrameBufferNames[i] == _bufferName) return true;
	}
	return false;
}

Material * AssetManager::GetDefaultMaterial()
{
	CreateDefaultMaterial();
	return &m_materials.at("default");
}
