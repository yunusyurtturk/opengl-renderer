#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoords;

uniform mat4 gVP;
uniform mat4 gWP;

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    FragPos = vec3(gWP * vec4(aPosition, 1.0));
    Normal = mat3(transpose(inverse(gWP))) * aNormal;  
    TexCoords = aTexCoords;

    gl_Position = gVP * vec4(FragPos, 1.0);
}