#version 430 core 
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
    //FragColor = vec4(0.8, 0.5, 0.3, 1.0);
}