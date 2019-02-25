#pragma once

#include <vector>

#include "Vertex.h"

class Mesh
{

public:

	Mesh();
	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indeces);
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indeces;
};