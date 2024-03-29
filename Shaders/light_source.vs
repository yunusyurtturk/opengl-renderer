#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

uniform mat4 gVP;
uniform mat4 gWP;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    gl_Position =  gVP * gWP * vec4(aPosition, 1.0);
} 