#include "Rendering/Shader.h"
#include <fstream>
#include <vector>
#include <filesystem>
#include <iostream>

VEngine::Shader::Shader(){}
VEngine::Shader::~Shader() { glDeleteShader(ShaderID); }

int VEngine::Shader::CompileShader()
{
	int Shader = glCreateShader(VShaderTypeToGLShaderEnum(ShaderType));
	std::ifstream ShaderReader;
	ShaderReader.open(ShaderPath, std::ios::binary | std::ios::ate);
	if (ShaderReader.is_open())
	{
		std::size_t end = ShaderReader.tellg();
		ShaderReader.seekg(0, std::ios::beg);
		std::vector<char> ShaderData;
		ShaderData.resize(end);
		ShaderReader.read(ShaderData.data(), end);
		char* RawData = ShaderData.data();
		int RawSize = ShaderData.size();

		glShaderSource(Shader, 1, &RawData, &RawSize);
		glCompileShader(Shader);
		ShaderID = Shader;
		std::cout << "Compiled shader " << ShaderPath << " with ID " << ShaderID << std::endl;
		return glGetError();
	}
	return -1;
}

int VEngine::Shader::SetShaderSource(const std::string& inShaderPath, const EShaderType inShaderType)
{
	std::ifstream ShaderReader;
	ShaderReader.open(inShaderPath);
	if (ShaderReader.is_open())
	{
		ShaderType = inShaderType;
		ShaderPath = inShaderPath;
		return 0;
	}
	else
	{
		std::cout << "File " << inShaderPath << " does not exist!" << std::endl;
		return -1;
	}
}