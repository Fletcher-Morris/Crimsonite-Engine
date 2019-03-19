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

#include "Texture.h"

class Shader
{
	
public:

	int ShaderId;
	
	Shader();
	Shader(const char* _vertexPath, const char* _fragmentPath);
	Shader(const char* _comboPath);
	Shader(const std::string _comboPath);

	void Compile(std::string _vertexCode, std::string _fragmentCode);
	void Compile(const std::string _comboPath);

	void Bind() const;
	void Unbind() const;

	void SetMvpMatrix(const glm::mat4 _mvp);
	void SetBool(const std::string& _name, bool _value);
	void SetInt(const std::string& _name, int _value);
	void SetFloat(const std::string& _name, float _value);
	void SetColor(const glm::vec3& _color);
	void SetColor(const glm::vec4& _color);
	void SetVector2(const std::string& _name, glm::vec2 _value);
	void SetVector3(const std::string& _name, glm::vec3 _value);
	void SetVector4(const std::string& _name, glm::vec4 _value);
	void SetTexture(const std::string& _name, Texture * _texture);

private:

	int GetUniformLocation(const std::string & _uniformName);
	std::unordered_map<std::string, int> m_locations;

};

struct ShaderCode
{
	std::string vertexCode;
	std::string fragmentCode;
};