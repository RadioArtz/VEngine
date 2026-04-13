#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace VEngine 
{
	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();
	private:
		int ProgramID = -1;

		void UpdateUniform(int inUniformLocation, float inValue) { glUniform1f(inUniformLocation, inValue); }
		void UpdateUniform(int inUniformLocation, int inValue) { glUniform1i(inUniformLocation, inValue); }
		void UpdateUniform(int inUniformLocation, glm::vec2 inValue) { glUniform2f(inUniformLocation, inValue.x, inValue.y); }
		void UpdateUniform(int inUniformLocation, glm::vec3 inValue) { glUniform3f(inUniformLocation, inValue.x, inValue.y, inValue.z); }
		void UpdateUniform(int inUniformLocation, glm::vec4 inValue) { glUniform4f(inUniformLocation, inValue.x, inValue.y, inValue.z, inValue.w); }
		void UpdateUniform(int inUniformLocation, glm::mat4 inValue) { glUniformMatrix4fv(inUniformLocation, 1, GL_FALSE, glm::value_ptr(inValue)); }


	public:
		int GetProgramID() const { return ProgramID; }
		int AttachShader(int inShaderID);
		int LinkProgram();
		int GetUniformIndex(const std::string& inUniformName);
		int GetAttributeIndex(const std::string& inAttributeName);

		bool IsReady() const { return (ProgramID != -1); };

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
		}

		template <typename T> int SetShaderValue(int uniformLocation, T inValue)
		{
			UpdateUniform(uniformLocation, inValue);
			return glGetError();
		}
	};
}