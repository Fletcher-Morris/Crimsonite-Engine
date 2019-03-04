#pragma once

#include <vector>

#include "Vertex.h"

class Mesh
{

public:

	Mesh();
	//	Constructor for mesh using vertices and indices parameters.
	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices);
	//	A vector holding the unique vertices used in the mesh.
	std::vector<Vertex> vertices;
	//	A vector holding the indices of vertices used in the mesh.
	std::vector<unsigned int> indices;

	void UploadToGpu();

	bool IsInitialized() { return m_initialized; }

private:

	//	The VAO location on the GPU.
	unsigned int VAO;
	//	The VBO location on the GPU.
	unsigned int VBO;
	//	The EBO location on the EBO.
	unsigned int EBO;

	//	The number of verteces in the mesh.
	int m_vertexCount = 0;
	//	The number on indeces in the mesh.
	int m_indexCount = 0;

	//	The size of a single vertex.
	unsigned long long m_sizeOfVertex;

	//	Has the mesh been initialized and sent to the gpu?
	bool m_initialized = false;
};