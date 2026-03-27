#include "Rendering/Material.h"

int Material::QueryParams()
{
    if (!activeShader.IsReady())
    {
        std::cout << "Material not ready for querying!" << std::endl;
        return -1;
    }

    int num;
    glGetProgramiv(activeShader.GetProgramID(), GL_ACTIVE_UNIFORMS, &num);
    uniformCache.reserve(num);
    std::cout << "Program " << "Basic2DShader" << " has " << num << " Uniforms" << std::endl;
    UniformMetadata tmp;
    for (int i = 0; i < num; i++)
    {
        char outName[64];
        int realLength = 0;
        GLenum paramType;
        int uniformSize = 0;
        glGetActiveUniform(activeShader.GetProgramID(), i, sizeof(outName), &realLength, &uniformSize, &paramType, outName);
        tmp.uniformName = outName;
        tmp.uniformSize = uniformSize;
        tmp.uniformType = paramType;
        tmp.uniformLocation = activeShader.GetUniformIndex(outName);
        uniformCache.insert(std::pair<std::string, UniformMetadata>(outName, tmp));
        std::cout << tmp.uniformLocation << " : " << outName << " | type: " << Shader::GlTypeToString(paramType) << std::endl;
    }
    return num;
};

int Material::InitializeMaterial(ShaderDef* inShaderDef)
{
    if (inShaderDef)
        shaderDef = *inShaderDef;

    Shader vert = Shader();
    Shader frag = Shader();

    vert.SetShaderSource(shaderDef.VertexPath, EShaderType::Vertex);
    frag.SetShaderSource(shaderDef.FragmentPath, EShaderType::Fragment);
    vert.CompileShader();
    frag.CompileShader();

    activeShader.AttachShader(vert.GetShaderID());
    activeShader.AttachShader(frag.GetShaderID());
    activeShader.LinkProgram();

    if (!activeShader.IsReady())
        return -1;

    QueryParams();
    return 0;
};

int Material::SetTextureParam(const std::string& inParamName, Texture* inTexture)
{
    auto uniformSearch = uniformCache.find(inParamName);
    if (uniformSearch == uniformCache.end())
    {
        std::cout << "Texture Parameter '" << inParamName << "' not found in ShaderProgram ProgramID:" << activeShader.GetProgramID();
        return -1;
    }
    if (uniformSearch->second.uniformType != GL_SAMPLER_2D)
    {
        std::cout << "Uniform '" << inParamName << "' is not of type GL_SAMPLER_2D ProgramID:" << activeShader.GetProgramID();
        return -1;
    }

    auto texBindSearch = textureBindings.find(inParamName);
    if (texBindSearch == textureBindings.end())
    {
        textureBindings[inParamName] = TextureBinding(inParamName, inTexture);
        std::cout << "bound new param '" << inParamName << "' to new slot " << std::endl;
        return 0;
    }
    else
    {
        if (texBindSearch->second.texturePtr == inTexture)
        {
            std::cout << "Exact Texture already bound to slot '" << inParamName << "'! Skipping.. " << std::endl;
            return 1;
        }
        textureBindings[inParamName] = TextureBinding(inParamName, inTexture);
        std::cout << "rebound param '" << inParamName << "' with new texture" << std::endl;
    }
};

void Material::RenderMaterial() 
{
    glUseProgram(activeShader.GetProgramID());
    int textureUnit = 0;
    int numTexBindings = textureBindings.size();

    if(numTexBindings == 0)
    {
        //no textures used. maybe warn IF there are supposed to be textures!
    }
    else 
    {
        for (auto& [name, binding] : textureBindings)
        {
            binding.texturePtr->ActivateTexture(textureUnit);
            int location = uniformCache[name].uniformLocation;
            glUniform1i(location, textureUnit);
            textureUnit++;
        }
    }
    
}