#include "Shader.h"
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
}

void Shader::Compile(const std::string _comboPath)
{

}

void Shader::Bind() const
{
}

void Shader::Unbind() const
{
}