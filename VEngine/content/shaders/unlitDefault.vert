#version 330 core
layout (location = 0) in vec3 aPos;
  
out vec3 VertexColor;

//all engine-provided uniforms lower camelCase with leading underscore
uniform float _time;

void main()
{
    gl_Position = vec4(aPos * (sin(_time + aPos) + 2) * 0.25f, 1.0);
    VertexColor = aPos;
}