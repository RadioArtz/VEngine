#include "Rendering/ShaderProgram.h"
#include <vector>
#include <iostream>

ShaderProgram::ShaderProgram() { ProgramID = glCreateProgram(); }
ShaderProgram::~ShaderProgram() { glDeleteProgram(ProgramID); }

int ShaderProgram::AttachShader(int inShaderID)
{
	glAttachShader(ProgramID, inShaderID);
	return glGetError();
}

int ShaderProgram::LinkProgram()
{
	glLinkProgram(ProgramID);
	return glGetError();
}

int ShaderProgram::GetUniformIndex(const std::string& inUniformName)
{
	return glGetUniformLocation(ProgramID, inUniformName.c_str());
}
/*
template <typename T> int ShaderProgram::SetShaderValue(const std::string& inUniformName, T inValue)
{
	int uniformLocation = GetUniformIndex(inUniformName);
	if (uniformLocation == -1)
	{
		std::cout << "Uniform " << inUniformName << " does not exist in shader program" << std::endl;
		return -1;
	}

	//glUniform1f(uniformLocation, inValue);
	UpdateUniform(uniformLocation, inValue);
	return glGetError();
}
*/