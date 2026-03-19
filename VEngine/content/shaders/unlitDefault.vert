#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 VertexColor;
out vec2 TexCoord;
//all engine-provided uniforms lower camelCase with leading underscore
uniform float _time;

void main()
{
    gl_Position = vec4(aPos * (sin(_time + aPos) + 2) * 0.25f, 1.0);
    VertexColor = aColor;
    TexCoord = aTexCoord;
}