#version 330 core
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;

uniform float _time;
uniform sampler2D _MainTex;
uniform sampler2D _OverlayTex;

void main()
{
    //FragColor = vec4(0.75, 0.3, 0.1, 1.0);
    //FragColor = abs(vec4(VertexColor, 1.0)) * ((cos(3.14f*_time)+1)/2);
    float lighting = (clamp(Normal.x + Normal.y + Normal.z,0.0,1.0)/2) + 0.25f;
    vec4 tex = texture(_MainTex, TexCoord) * lighting;
    //FragColor = vec4(VertexColor * abs(sin(_time)), 1.0) * tex;
    //FragColor = mix(tex, texture(_OverlayTex, TexCoord)*tex, 1-(sin(_time)+1)/2);;
    FragColor = tex;
}