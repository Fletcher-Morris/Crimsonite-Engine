#include "Shader.h"

Shader::Shader()
{
}

Shader::Shader(const char * _vertexPath, const char * _fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertexFile.open(_vertexPath);
		fragmentFile.open(_fragmentPath);

		std::stringstream vertexStream;
		std::stringstream fragmentStream;

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();

		std::cout << "Loaded shader file : " + (std::string)_vertexPath << std::endl;
		std::cout << "Loaded shader file : " + (std::string)_fragmentPath << std::endl;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Failed to load shader file : " + (std::string)_vertexPath << std::endl;
		std::cout << "Failed to load shader file : " + (std::string)_fragmentPath << std::endl;
	}

	Compile(vertexCode, fragmentCode);
}

Shader::Shader(const char * _comboPath)
{
}

Shader::Shader(const std::string _comboPath)
{
}

void Shader::Compile(std::string _vertexCode, std::string _fragmentCode)
{
	const char * vertexCode = _vertexCode.c_str();
	const char * fragmentCode = _fragmentCode.c_str();

	unsigned int vertexShader;
	unsigned int fragmentShader;

	int success;
	char info[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << "Failed to compile vertex shader :\n" << info << std::endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << "Failed to compile fragment shader :\n" << info << std::endl;
	}

	shaderId = glCreateProgram();
	glAttachShader(shaderId, vertexShader);
	glAttachShader(shaderId, fragmentShader);
	glLinkProgram(shaderId);

	glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderId, 512, NULL, info);
		std::cout << "Failed to link shader :\n" << info << std::endl;
	}
	std::cout << "Linked shader : " << shaderId << std::endl;

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Compile(const std::string _comboPath)
{

}

void Shader::Bind() const
{
	if (shaderId == 0) return;
	glUseProgram(shaderId);
}

void Shader::Unbind() const
{
	if (shaderId == 0) return;
	glUseProgram(0);
}

void Shader::SetMvpMatrix(const glm::mat4 _mvp)
{
	if (shaderId == 0) return;
	glUniformMatrix4fv(GetUniformLocation("u_MVP"), 1, GL_FALSE, glm::value_ptr(_mvp));
}

void Shader::SetBool(const std::string & _name, bool _value)
{
	if (shaderId == 0) return;
	glUniform1i(glGetUniformLocation(shaderId, _name.c_str()), (int)_value);
}

void Shader::SetInt(const std::string & _name, int _value)
{
	if (shaderId == 0) return;
	glUniform1i(glGetUniformLocation(shaderId, _name.c_str()), (int)_value);
}

void Shader::SetFloat(const std::string & _name, float _value)
{
	if (shaderId == 0) return;
	glUniform1i(glGetUniformLocation(shaderId, _name.c_str()), _value);
}

void Shader::SetColor(const glm::vec3 & _color)
{
	if (shaderId == 0) return;
	glUniform4f(GetUniformLocation("u_Color"), _color.x, _color.y, _color.z, 1.0f);
}

void Shader::SetColor(const glm::vec4 & _color)
{
	if (shaderId == 0) return;
	glUniform4f(GetUniformLocation("u_Color"), _color.x, _color.y, _color.z, _color.w);
}

void Shader::SetVector2(const std::string & _name, glm::vec2 _value)
{
	if (shaderId == 0) return;
	glUniform2f(GetUniformLocation(_name), _value.x, _value.y);
}

void Shader::SetVector3(const std::string & _name, glm::vec3 _value)
{
	if (shaderId == 0) return;
	glUniform3f(GetUniformLocation(_name), _value.x, _value.y, _value.z);
}

void Shader::SetVector4(const std::string & _name, glm::vec4 _value)
{
	if (shaderId == 0) return;
	glUniform4f(GetUniformLocation(_name), _value.x, _value.y, _value.z, _value.w);
}

unsigned int Shader::GetUniformLocation(const std::string & _uniformName)
{
	if (m_locations.find(_uniformName) != m_locations.end())
	{
		return m_locations[_uniformName];
	}
	int location = glGetUniformLocation(shaderId, _uniformName.c_str());
	if (location == -1)
	{
		std::cout << "Uniform '" << _uniformName << "' does not exist!" << std::endl;
	}
	m_locations[_uniformName] = location;
	return location;
}
