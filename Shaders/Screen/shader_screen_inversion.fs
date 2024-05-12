#version 430 core 
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec3 textures = 1.0 - texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(textures, 1.0);
}