#pragma once

#include <string>

class Shader
{
	
public:

	unsigned int shaderId;
	
	Shader();
	Shader(const char* _vertexPath, const char* _fragmentPath);
	Shader(const std::string _vertexPath, const std::string _fragmentPath);
	Shader(const char* _comboPath);
	Shader(const std::string _comboPath);

	void Bind() const;
	void Unbind() const;
};