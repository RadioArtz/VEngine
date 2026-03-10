#include "Rendering/ShaderProgram.h"
#include <vector>

ShaderProgram::ShaderProgram() { ProgramID = glCreateProgram(); }
ShaderProgram::~ShaderProgram() { glDeleteProgram(ProgramID); }

int ShaderProgram::AttachShader(int inShaderID)
{
	glAttachShader(ProgramID, inShaderID);
	return 0;
}

int ShaderProgram::LinkProgram()
{
	glLinkProgram(ProgramID);
	return 0;
}
