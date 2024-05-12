#version 410 core

in vec3 Color;
in vec2 TexCoord;

in float Height;

uniform sampler2D gSampler;

out vec4 FragColor;


void main()
{
  FragColor = vec4(1.0, 0.5, 0.5, 1.0);
}