#pragma once
#include <string>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

class CompiledShaderProgram
{
public:
    GLuint ShaderProgram;
    std::string VertexShader;
    std::string FragmentShader;
};
