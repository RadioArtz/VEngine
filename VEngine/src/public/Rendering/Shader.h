#pragma once
#include <string>
#include <glad/glad.h>

enum class EShaderType
{
	Fragment = 0,
	Vertex = 1
};

class Shader
{
public:
	Shader();
	~Shader();

private:
	EShaderType ShaderType;
	std::string ShaderPath;
	int ShaderID = -1;

public:
	int GetShaderID() { return ShaderID; };
	int CompileShader();
	int SetShaderSource(const std::string& inShaderPath, const EShaderType inShaderType);
	static int VShaderTypeToGLShaderEnum(const EShaderType inShaderType)
	{
		switch (inShaderType)
		{
		case EShaderType::Fragment: return GL_FRAGMENT_SHADER;
		case EShaderType::Vertex: return GL_VERTEX_SHADER;
		};
	};
};

class ShaderProgram 
{
public:
	ShaderProgram();
	~ShaderProgram();
private:
	int ProgramID = -1;
public:
	int GetProgramID() { return ProgramID; };
	int AttachShader(int inShaderID);
	int LinkProgram();
};
