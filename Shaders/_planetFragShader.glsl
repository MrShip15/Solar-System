#version 330 core

const vec3 lightColor = vec3(0.8, 0.8, 0.8);
const vec3 lightPosition = vec3(0.0, 0.0, 0.0);
const vec3 ambientLight = vec3(0.3, 0.3, 0.3);

in vec3 vPosition;
in vec3 vNormal;

uniform vec4 uColor;

out vec4 fragColor;

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(lightPosition - vPosition);
    float nDotL = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = lightColor * uColor.rgb * nDotL;
    vec3 ambient = ambientLight * uColor.rgb;
    fragColor = vec4(diffuse + ambient, uColor.a);
}
