#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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
};

struct ShaderCode
{
	std::string vertexCode;
	std::string fragmentCode;
};