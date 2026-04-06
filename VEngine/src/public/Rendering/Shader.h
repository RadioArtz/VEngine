#pragma once
#include <string>
#include <glad/glad.h>

struct ShaderDef 
{
	ShaderDef() = default;
	ShaderDef(std::string inName, std::string inVertPath, std::string inFragPath)
			: Name(std::move(inName)), VertexPath(std::move(inVertPath)), FragmentPath(std::move(inFragPath)){}
	std::string Name = "defaultShader";
	std::string VertexPath = "game/content/shaders/unlitDefault.vert";
	std::string FragmentPath = "game/content/shaders/unlitDefault.frag";
};

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
	std::string ShaderPath{};
	int ShaderID = -1;

public:
	int GetShaderID() const { return ShaderID; };
	int CompileShader();
	int SetShaderSource(const std::string& inShaderPath, const EShaderType inShaderType);
	static int VShaderTypeToGLShaderEnum(const EShaderType inShaderType)
	{
		switch (inShaderType)
		{
		case EShaderType::Fragment: return GL_FRAGMENT_SHADER;
		case EShaderType::Vertex: return GL_VERTEX_SHADER;
		};
	}
	bool IsCompiled() const { return (ShaderID != -1); }

	static const char* GlTypeToString(GLenum type) {
		switch (type) 
		{
		case GL_FLOAT:	return "float";
		case GL_FLOAT_VEC2:	return "vec2";
		case GL_FLOAT_VEC3:	return "vec3";
		case GL_FLOAT_VEC4:	return "vec4";
		case GL_INT:	return "int";
		case GL_BOOL:	return "bool";
		case GL_FLOAT_MAT4:	return "mat4";
		case GL_SAMPLER_2D:	return "sampler2D";
		default: return "undefined uniform type!";
		}
	}
};