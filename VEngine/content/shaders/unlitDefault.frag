#version 330 core
in vec3 VertexColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform float _time;
uniform sampler2D _MainTex;
 
void main()
{
    //FragColor = vec4(0.75, 0.3, 0.1, 1.0);
    //FragColor = abs(vec4(VertexColor, 1.0)) * ((cos(3.14f*_time)+1)/2);
    vec4 tex = texture(_MainTex, TexCoord);
    FragColor = vec4(VertexColor * abs(sin(_time)), 1.0) * tex;
}