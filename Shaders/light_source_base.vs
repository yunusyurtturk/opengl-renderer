#version 430 core

layout (location = 0) in vec3 aPosition;

uniform mat4 gVP;
uniform mat4 gWP;


void main()
{
    gl_Position =  gVP * gWP * vec4(aPosition, 1.0);
} 