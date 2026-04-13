#pragma once
#include <Rendering/Shader.h>
#include <Rendering/ShaderProgram.h>
#include <vector>
#include <Rendering/Texture.h>
#include <variant>


namespace VEngine
{
	using UniformVal = std::variant<std::monostate, int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, Texture*>;

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
		UniformVal uniformValue;
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

		int QueryParams();
	public:
		void SetShader(ShaderDef inShader) {};
		ShaderDef GetShaderInfo() { return shaderDef; };
		ShaderProgram* GetActiveShader() { return &activeShader; };

		int InitializeMaterial(ShaderDef* inShaderDef = nullptr);

		template <typename T> int SetMaterialParam(const std::string& inParamName, T inValue)
		{
				return SetValueParamCached(inParamName, (UniformVal)inValue);
		}

		void RenderMaterial();

	private:

		int SetValueParamCached(const std::string& inParamName, UniformVal inValue);
	};
}