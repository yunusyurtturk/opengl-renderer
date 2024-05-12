#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <string>
#include "Utils.h"
#include "CompiledShaderProgram.h"


class ShaderCompiler
{
private:
    GLuint ShaderProgram;
public:
    static void AddShader(GLuint ShaderProgram, const char* pShaderText, const std::string &fileName, GLenum ShaderType)
    {
        GLuint ShaderObj = glCreateShader(ShaderType);

        if (ShaderObj == 0) {
            fprintf(stderr, "Error creating shader type. %d, File: %s\n", ShaderType, fileName.c_str());
            exit(0);
        }

        const GLchar* p[1];
        p[0] = pShaderText;

        GLint Lengths[1];
        Lengths[0] = (GLint)strlen(pShaderText);

        glShaderSource(ShaderObj, 1, p, Lengths);

        glCompileShader(ShaderObj);

        GLint success;
        glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

        if (!success) {
            GLchar InfoLog[1024];
            glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
            fprintf(stderr, "Error compiling shader type %d File: %s: '%s'\n", ShaderType, fileName.c_str(), InfoLog);
            exit(1);
        }

        glAttachShader(ShaderProgram, ShaderObj);
    }
    CompiledShaderProgram CompileShaders(
        const std::string &vertexShaderFile, 
        const std::string &fragmentShaderFile, 
        const std::string& tessellationControlFile = "", 
        const std::string& tessellationEvalFile = "")
    {
        CompiledShaderProgram retShaderProgram;
        GLuint ShaderProgram = -1;
        std::string vertexShader;
        std::string fragmentShader;
        std::string tessellationControl;
        std::string tessellationEval;
        ShaderProgram = glCreateProgram();
        std::string vs, fs;

        if (ShaderProgram == 0) {
            printf("Error creating shader program glCreateProgram()");
            exit(1);
        }

        if (!ReadFile(vertexShaderFile.c_str(), vertexShader)) {
            std::cout << "Vertex shader file read problem:" << vertexShaderFile << "\n";
            exit(1);
        }

        AddShader(ShaderProgram, vertexShader.c_str(), vertexShaderFile, GL_VERTEX_SHADER);

        if (!ReadFile(fragmentShaderFile.c_str(), fragmentShader)) {
            std::cout << "Fragment shader file read problem:" << vertexShaderFile << "\n";
            exit(1);
        }
        AddShader(ShaderProgram, fragmentShader.c_str(), fragmentShaderFile, GL_FRAGMENT_SHADER);

        if (tessellationControlFile != "") {
            if (!ReadFile(tessellationControlFile.c_str(), tessellationControl)) {
                std::cout << "Tessellation control shader file read problem:" << vertexShaderFile << "\n";
                exit(1);
            }
            AddShader(ShaderProgram, tessellationControl.c_str(), tessellationControlFile, GL_TESS_CONTROL_SHADER);
        }

        if (tessellationEvalFile != "") {
            if (!ReadFile(tessellationEvalFile.c_str(), tessellationEval)) {
                std::cout << "Tessellation evaluation shader file read problem:" << vertexShaderFile << "\n";
                exit(1);
            }
            AddShader(ShaderProgram, tessellationEval.c_str(), tessellationEvalFile, GL_TESS_EVALUATION_SHADER);
        }

        GLint Success = 0;
        GLchar ErrorLog[1024] = { 0 };

        glLinkProgram(ShaderProgram);
        glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
        if (Success == 0) {
            glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
            exit(1);
        }
/*
        gVPLocation = glGetUniformLocation(ShaderProgram, "gVP");
        if (gVPLocation == -1) {
            printf("Error getting uniform location 'gVP'\n");
            exit(1);
        }

        gWLocation = glGetUniformLocation(ShaderProgram, "gW");
        if (gWLocation == -1) {
            printf("Error getting uniform location 'gW'\n");
            // exit(1);
        }

        gSamplerLocation = glGetUniformLocation(ShaderProgram, "gSampler");
        if (gSamplerLocation == -1) {
            printf("Error getting uniform location 'gSampler'\n");
            //   exit(1);
        }
*/

        glValidateProgram(ShaderProgram);
        glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);

        if (!Success) {
            glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
            exit(1);
        }

        glUseProgram(ShaderProgram);

        retShaderProgram.ShaderProgram = ShaderProgram;
        retShaderProgram.VertexShader = vertexShader;
        retShaderProgram.FragmentShader = fragmentShader;

        return retShaderProgram;
    }
};