#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
private:
	int ProgramID = -1;

	void UpdateUniform(int inUniformLocation, float inValue) { glUniform1f(inUniformLocation, inValue); };
	void UpdateUniform(int inUniformLocation, int inValue) { glUniform1i(inUniformLocation, inValue); };
	void UpdateUniform(int inUniformLocation, glm::vec2 inValue) { glUniform2f(inUniformLocation, inValue.x, inValue.y); };
	void UpdateUniform(int inUniformLocation, glm::vec3 inValue) { glUniform3f(inUniformLocation, inValue.x, inValue.y, inValue.z); };
	void UpdateUniform(int inUniformLocation, glm::vec4 inValue) { glUniform4f(inUniformLocation, inValue.x, inValue.y, inValue.z, inValue.w); };


public:
	int GetProgramID() { return ProgramID; };
	int AttachShader(int inShaderID);
	int LinkProgram();
	int GetUniformIndex(const std::string& inUniformName);

	template <typename T> int SetShaderValue(const std::string& inUniformName, T inValue)
	{
		int uniformLocation = GetUniformIndex(inUniformName);
		if (uniformLocation == -1)
		{
			std::cout << "Uniform " << inUniformName << " does not exist in shader program" << std::endl;
			return -1;
		}

		UpdateUniform(uniformLocation, inValue);
		return glGetError();
	};
};