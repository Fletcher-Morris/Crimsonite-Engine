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

	Material();
	Material(std::string _name);

	std::map<std::string, bool> boolProperties;
	std::map<std::string, int> intProperties;
	std::map<std::string, float> floatProperties;
	std::map<std::string, glm::vec2> vec2Properties;
	std::map<std::string, glm::vec3> vec3Properties;
	std::map<std::string, glm::vec4> vec4Properties;
	std::map<std::string, std::string> textureProperties;

	void UpdateShaderProperties();

	void SetName(std::string _name) { m_name = _name; }
	std::string GetName() { return m_name; }
	Shader * GetShader() { return m_shader; }
	void SetShader(Shader * _shader) { m_shader = _shader; }
	void SetShader(std::string _shader);
	void SetColor(glm::vec3 _color);
	void SetColor(glm::vec4 _color);

private:

	std::string m_name = "New Material";
	Shader * m_shader;
	glm::vec4 m_color;
};