#version 330 core
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;
out vec4 FragColor;

uniform float _time;
uniform vec3 _ambientColor;
uniform vec3 _viewPos;

uniform sampler2D _MainTex;
uniform sampler2D _EmissiveTex;

uniform vec3 diffuseColor;

uniform vec3 diffuseLightColor;
uniform vec3 specularLightColor;
uniform float specularHardness;
uniform vec3 lightPosition;

void main()
{
    vec4 diffuseTex = texture(_MainTex, TexCoord);
    vec4 emissiveTex = texture(_EmissiveTex, TexCoord);

    //ambient
    float ambientStrength = 1f;
    vec3 ambientLight = ambientStrength * _ambientColor;

    //diff
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);  

    float diffuseStrength = max(dot(norm, lightDir),0.0);
    vec3 diffuseLight = diffuseStrength * diffuseLightColor;

    //spec
    vec3 viewDir = normalize(_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularHardness);
    vec3 specularLight = spec * specularLightColor;

    //final
    vec3 lightResult = ambientLight + diffuseLight + specularLight;

    vec3 result = lightResult * diffuseColor * vec3(diffuseTex.x,diffuseTex.y,diffuseTex.z);

    FragColor = vec4(result,1.0f) + emissiveTex;
}