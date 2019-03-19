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
	m_vertexSize = sizeof(Vertex);

	//	Set up GPU buffers.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * m_vertexSize, &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//	Vetex positions.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_vertexSize, (void*)0);
	glEnableVertexAttribArray(0);

	//	Vertex normals.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, m_vertexSize, (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	//	Vertex UV.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, m_vertexSize, (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);

	//	Unbind GPU buffers and arrays.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_initialized = true;
}