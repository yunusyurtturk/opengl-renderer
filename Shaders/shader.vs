#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 gVP;
uniform mat4 gWP;

out vec3 Color;
out vec2 TexCoord0;

void main()
{
    gl_Position =  gVP * gWP * vec4(aPosition, 1.0);
    Color = aColor;
    TexCoord0 = aTexCoord;
}