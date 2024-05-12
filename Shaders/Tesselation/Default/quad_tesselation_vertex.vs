#version 410 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;

void main()
{
    gl_Position = vec4(aPosition, 1.0);
    TexCoord = aTexCoord;
    Normal = aNormal;


}