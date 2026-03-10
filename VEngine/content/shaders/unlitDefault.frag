#version 330 core
in vec3 VertexColor;
out vec4 FragColor;

uniform float _time;
 
void main()
{
    //FragColor = vec4(0.75, 0.3, 0.1, 1.0);
    FragColor = abs(vec4(VertexColor, 1.0)) * ((cos(3.14f*_time)+1)/2);
}