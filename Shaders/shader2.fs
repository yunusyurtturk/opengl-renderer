#version 430

in vec3 Color;
in vec2 TexCoord0;
uniform sampler2D gSampler;

out vec4 FragColor;


void main()
{
  vec4 textureColor = texture(gSampler, TexCoord0);
  FragColor = vec4(textureColor.r, textureColor.r, textureColor.r, 1.0);
}