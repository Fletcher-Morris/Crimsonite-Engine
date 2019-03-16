#pragma once

#include <map>
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Shader.h"

class Material
{

public:

	std::map<std::string, bool> boolProperties;
	std::map<std::string, int> intProperties;
	std::map<std::string, float> floatProperties;
	std::map<std::string, glm::vec2> vec2Properties;
	std::map<std::string, glm::vec3> vec3Properties;
	std::map<std::string, glm::vec4> vec4Properties;


private:

	std::string m_name = "New Material";
	Shader * m_shader;
};