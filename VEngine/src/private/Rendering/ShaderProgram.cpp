#include "Rendering/ShaderProgram.h"
#include <vector>
#include <iostream>

VEngine::ShaderProgram::ShaderProgram() { ProgramID = glCreateProgram(); }
VEngine::ShaderProgram::~ShaderProgram() { glDeleteProgram(ProgramID); }

int VEngine::ShaderProgram::AttachShader(int inShaderID)
{
	glAttachShader(ProgramID, inShaderID);
	return glGetError();
}

int VEngine::ShaderProgram::LinkProgram()
{
	glLinkProgram(ProgramID);
	return glGetError();
}

int VEngine::ShaderProgram::GetUniformIndex(const std::string& inUniformName)
{
	int uniformLocation = glGetUniformLocation(ProgramID, inUniformName.c_str());
	if (uniformLocation == -1)
		std::cout << "Uniform " << inUniformName << " does not exist in shader program" << std::endl;
	return uniformLocation;
}

int VEngine::ShaderProgram::GetAttributeIndex(const std::string& inAttributeName)
{
	int attributeLocation = glGetAttribLocation(ProgramID, inAttributeName.c_str());
	if (attributeLocation == -1)
		std::cout << "Attribute " << inAttributeName << " does not exist in shader program" << std::endl;
	return attributeLocation;
}