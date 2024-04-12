#pragma once
#include <string>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Base/Uniform/UniformElement.h"

class CompiledShaderProgram
{
private:
    
public:
    GLuint ShaderProgram = -1;
    std::string VertexShader;
    std::string FragmentShader;

};
