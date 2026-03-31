#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

//all engine-provided uniforms lower camelCase with leading underscore
uniform float _time;
uniform mat4 _model, _view, _projection;

void main()
{
    gl_Position = _projection * _view * _model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(_model))) * aNormal;  
    TexCoord = aTexCoord;
    FragPos = vec3(_model * vec4(aPos, 1.0));
}