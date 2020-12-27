#version 330 core

uniform sampler2D uSampler;
in vec2 vTexCoord;
out vec4 fragColor;

void main()
{
    fragColor = texture2D(uSampler, vTexCoord);
}
