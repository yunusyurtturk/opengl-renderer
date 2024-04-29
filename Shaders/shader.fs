#version 330

in vec3 Color;
in vec2 TexCoord0;
uniform sampler2D gSampler;

out vec4 FragColor;


void main()
{
  FragColor = texture(gSampler, TexCoord0);
  //FragColor = vec4(texture2D(gSampler, TexCoord0), 1.0);
}