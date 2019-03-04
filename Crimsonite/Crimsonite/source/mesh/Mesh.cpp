#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indeces)
{
	this->vertices = _vertices;
	this->indices = _indeces;

	UploadToGpu();
}

void Mesh::UploadToGpu()
{
	m_vertexCount = vertices.size();
	m_indexCount = indices.size();
	m_sizeOfVertex = sizeof(Vertex);

	glGenVertexArrays(1, &VAO);

	m_initialized = true;
}