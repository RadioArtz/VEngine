#pragma once
#include <Rendering/Shader.h>
#include <Rendering/ShaderProgram.h>
#include <vector>
#include <Rendering/Texture.h>

struct UniformMetadata 
{
	UniformMetadata() {};
	UniformMetadata(std::string inUniformName, int inUniformSize, GLenum inUniformType) :
		uniformName(std::move(inUniformName)), uniformSize(std::move(inUniformSize)), uniformType(std::move(inUniformType)) {};
	std::string uniformName = "uninitialized";
	/// <summary>
	/// array length, 1 if single element
	/// </summary>
	int uniformSize = -1;
	GLenum uniformType = -1;
	int uniformLocation = -1;
};

struct TextureBinding 
{
	std::string uniformName = "uninitialized";
	Texture* texturePtr = nullptr;
};

class Material
{
public:
	Material() {};
	Material(ShaderDef inShaderDef, bool autoInitialize) 
	{
		std::cout << "hi" << std::endl;
		shaderDef = inShaderDef;
		if (autoInitialize) 
		{
			InitializeMaterial();
		}
	}

private:
	ShaderDef shaderDef;
	ShaderProgram activeShader;
	unsigned int numUniforms = 0;
	std::unordered_map<std::string, UniformMetadata> uniformCache;
	std::unordered_map<std::string, TextureBinding> textureBindings;

	int QueryParams();
public:
	void SetShader(ShaderDef inShader) {};
	ShaderDef GetShaderInfo() { return shaderDef; };
	ShaderProgram* GetActiveShader() { return &activeShader; };

	int InitializeMaterial(ShaderDef* inShaderDef = nullptr);

	int SetTextureParam(const std::string& inParamName, Texture* inTexture);

	void RenderMaterial();
};