#include "Rendering/Material.h"

int VEngine::Material::QueryParams()
{
    if (!activeShader.IsReady())
    {
        std::cout << "WARN | Material not ready for querying!" << std::endl;
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
}

int VEngine::Material::InitializeMaterial(ShaderDef* inShaderDef)
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
}

int VEngine::Material::SetValueParamCached(const std::string& inParamName, UniformVal inValue)
{
    auto uniformSearch = uniformCache.find(inParamName);
    if (uniformSearch == uniformCache.end())
    {
        std::cout << "Parameter '" << inParamName << "' not found in ShaderProgram with ID:" << activeShader.GetProgramID() << std::endl;
        return -1;
    }
    else
    {
        uniformSearch->second.uniformValue = inValue;
        //std::cout << "Set material parameter '" << inParamName << "' to slot" << std::endl;
        return 0;
    }
    return 0;
}

void VEngine::Material::RenderMaterial()
{
    glUseProgram(activeShader.GetProgramID());
    int textureUnit = 0;
    size_t numUniforms = uniformCache.size();
    int numUnboundTextures = 0;

    for (auto& [name, uniformData] : uniformCache) 
    {
        if (std::holds_alternative<std::monostate>(uniformData.uniformValue))
        {
            //Uniform supposed to hold a value but never assigned one!
            std::cout << "WARN | Uniform " << uniformData.uniformName << " does not have a value assigned!" << std::endl;
            continue;
        }

        int location = uniformCache[name].uniformLocation;
        switch (uniformData.uniformType)
        {
            case GL_INT: activeShader.SetShaderValue(location, std::get<int>(uniformData.uniformValue)); break;
            case GL_FLOAT: activeShader.SetShaderValue(location, std::get<float>(uniformData.uniformValue)); break;
            case GL_FLOAT_VEC2: activeShader.SetShaderValue(location, std::get<glm::vec2>(uniformData.uniformValue)); break;
            case GL_FLOAT_VEC3: activeShader.SetShaderValue(location, std::get<glm::vec3>(uniformData.uniformValue)); break;
            case GL_FLOAT_VEC4: activeShader.SetShaderValue(location, std::get<glm::vec4>(uniformData.uniformValue)); break;
            case GL_FLOAT_MAT4: activeShader.SetShaderValue(location, std::get<glm::mat4>(uniformData.uniformValue)); break;
            case GL_SAMPLER_2D:
            {
                (std::get<Texture*>(uniformData.uniformValue))->ActivateTexture(textureUnit);
                activeShader.SetShaderValue(location, textureUnit);
                textureUnit++;
            }
        }
    }
}