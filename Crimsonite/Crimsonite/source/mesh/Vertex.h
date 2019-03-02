#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
	//	XYZ coordinates of the vertex position.
	glm::vec3 position;
	//	XYZ coordinates of the vertex normal.
	glm::vec3 normal;
	//	XY coordinates of the vertex' uv mapping.
	glm::vec2 uv;
};

struct IndexTriplet
{
	int position;
	int normal;
	int uv;
};