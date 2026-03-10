#pragma once
#include <string>
#include <glad/glad.h>

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