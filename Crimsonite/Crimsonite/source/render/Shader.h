#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include <gl/glew.h>
#include <gl/GLU.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Shader
{
	
public:

	unsigned int shaderId;
	
	Shader();
	Shader(const char* _vertexPath, const char* _fragmentPath);
	Shader(const char* _comboPath);
	Shader(const std::string _comboPath);

	void Compile(std::string _vertexCode, std::string _fragmentCode);
	void Compile(const std::string _comboPath);

	void Bind() const;
	void Unbind() const;

	void SetMvpMatrix(const glm::mat4 _mvp);

private:

	unsigned int GetUniformLocation(const std::string & _uniformName);
	std::unordered_map<std::string, int> m_locations;

};

struct ShaderCode
{
	std::string vertexCode;
	std::string fragmentCode;
};