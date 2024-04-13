#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 gVP;
uniform mat4 gWP;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    gl_Position =  gVP * gWP * vec4(aPosition, 1.0);
} 