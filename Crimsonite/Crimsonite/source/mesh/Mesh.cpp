#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indeces)
{
	this->vertices = _vertices;
	this->indices = _indeces;
}

void Mesh::UploadToGpu()
{
	m_initialized = true;
}