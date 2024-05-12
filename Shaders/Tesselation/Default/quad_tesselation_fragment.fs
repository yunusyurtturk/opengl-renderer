#version 410 core

in float Height;
in vec2 FragTexCoord;
out vec4 FragColor;

uniform sampler2D gSampler;


void main()
{
    float h = texture(gSampler, FragTexCoord).r;
    FragColor = vec4(h, h, h, 1.0);
}