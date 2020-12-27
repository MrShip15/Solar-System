#version 330 core

in vec4 aPosition;
in vec4 aNormal;
uniform mat4 uMvpMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;
out vec3 vPosition;
out vec3 vNormal;

void main()
{
    gl_Position = uMvpMatrix * aPosition;
    vPosition = vec3(uModelMatrix * aPosition);
    vNormal = normalize(vec3(uNormalMatrix * aNormal));
}
