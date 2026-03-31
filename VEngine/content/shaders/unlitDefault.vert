#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
//all engine-provided uniforms lower camelCase with leading underscore
uniform float _time;
uniform mat4 _model, _view, _projection;

void main()
{
    //gl_Position = vec4(aPos * (sin(_time + aPos) + 2) * 0.25f, 1.0) * _transform;
    gl_Position = _projection * _view * _model * vec4(aPos, 1.0);
    Normal = aNormal;
    TexCoord = aTexCoord;
}