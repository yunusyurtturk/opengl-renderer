#version 430

layout (location = 0) in vec3 aPosition;

uniform mat4 gVP;
uniform mat4 gWP;

out vec3 TexCoord;

void main()
{
    TexCoord = aPosition;
    vec4 pos = gVP * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}