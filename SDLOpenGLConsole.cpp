// SDLOpenGLConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//#include <corecrt_math.h>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>


#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "WorldTransform.h"
#include "Camera.h"
#include "Texture.h"
#include "Utils.h"
#include "ShaderCompiler.h"
#include "CompiledShaderProgram.h"
#include "Object.h"
#include "Base/CubeLightSource.h"

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WinWidth 800
#define WinHeight 800

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

typedef glm::vec3 Vector3f;

GLuint VAO;
GLuint VBO;
GLuint IBO;
GLuint gWLocation;
GLuint texture;
GLuint ShaderProgram;

glm::vec3 CameraPos(0.0f, 0.0f, -3.0f);
glm::vec3 CameraTarget(0.0f, 0.0f, 1.0f);
glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);
Camera GameCamera(WinWidth, WinHeight, CameraPos, CameraTarget, CameraUp);

std::vector<Primitive *> Primitives;

float FOV = 45.0f;
float NearZ = 1.0f;
float FarZ = 10.0f;
float ar = (float)WinWidth / (float)(WinHeight);

static void RenderSceneCB( vector<Primitive *> &Primitives)
{
    float YRotationAngle = 1.0f;
    glClear(GL_COLOR_BUFFER_BIT);
    GameCamera.OnRender();

    Primitives[0]->GetTransform().Rotate(0.0f, YRotationAngle, 0.0f);
    Primitives[1]->GetTransform().Rotate(YRotationAngle, 0.0f, 0.0f);

    glm::mat4x4 View = GameCamera.GetMatrix();
    glm::mat4x4 Projection = glm::perspective(FOV, ar, NearZ, FarZ);
    glm::mat4x4 VP = Projection * View;

    for (auto& primitive : Primitives)
    {
        primitive->Update(VP);
    }

}

static void CreateVertexBuffer()
{
    Vertex Vertices[8];

    Vertices[0] = Vertex(0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    Vertices[1] = Vertex(-0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f);
    Vertices[2] = Vertex(-0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    Vertices[3] = Vertex(0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f);
    Vertices[4] = Vertex(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f);
    Vertices[5] = Vertex(0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f);
    Vertices[6] = Vertex(0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    Vertices[7] = Vertex(-0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


}

static void CreateIndexBuffer()
{
    unsigned int Indices[] = {
                              0, 1, 2,
                              1, 3, 4,
                              5, 6, 3,
                              7, 3, 6,
                              2, 4, 7,
                              0, 7, 6,
                              0, 5, 1,
                              1, 5, 3,
                              5, 0, 6,
                              7, 4, 3,
                              2, 1, 4,
                              0, 2, 7
    };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

}
#if 0
const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
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
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
    ShaderProgram = glCreateProgram();
    std::string vs, fs;

    if (ShaderProgram == 0) {
        printf("HATA");
        exit(1);
    }

    if (!ReadFile(pVSFileName, vs)) {
        exit(1);
    }

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    if (!ReadFile(pFSFileName, fs)) {
        exit(1);
    }
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);
    if (!ReadFile(pFSFileName, fs)) {
        exit(1);
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


    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);

    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);
}
#endif 

int main(int ArgCount, char** Args)
{
#ifdef _WIN64
    srand(1);
#else
    srandom(getpid());
#endif

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    u32 WindowFlags = SDL_WINDOW_OPENGL;
    SDL_Window* Window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WinWidth, WinHeight, WindowFlags);
    assert(Window);
    SDL_GLContext Context = SDL_GL_CreateContext(Window);

    b32 Running = 1;
    b32 FullScreen = 0;

    GLclampf bgcolor = 0.5f;

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }
    ShaderCompiler shaderCompiler;

    glEnable(GL_CULL_FACE);

    const std::string pVSFileName  = "./Shaders/shader.vs";
    const std::string pFSFileName  = "./Shaders/shader.fs";
    const std::string pFSFileName2 = "./Shaders/shader2.fs";
    const std::string pLightSourceFragmentShader = "./Shaders/light_source.fs";
    const std::string pLightSourceVertexShader = "./Shaders/light_source.vs";
    CompiledShaderProgram textureShader = shaderCompiler.CompileShaders(pVSFileName, pFSFileName);
    CompiledShaderProgram textureShader2 = shaderCompiler.CompileShaders(pVSFileName, pFSFileName2);
    CompiledShaderProgram lightShader = shaderCompiler.CompileShaders(pLightSourceVertexShader, pLightSourceFragmentShader);

    ModelObject Cube;
    Cube.AddShader(textureShader);
    Cube.SetTexture("bricks.jpg");
    Cube.SetPosition(0.0, 0.0f, -2.0f);
    Primitives.push_back(&Cube);

    ModelObject Cube2;
    Cube2.AddShader(textureShader2);
    Cube2.SetTexture("container.jpg");
    Cube2.SetPosition(0.0, -1.0f, -4.0f);
    Primitives.push_back(&Cube2);

    CubeLightSource Light;
    Light.AddShader(lightShader);
    Light.SetPosition(0.0, -2.0f, -3.0f);
    Primitives.push_back(&Light);

    while (Running)
    {
        SDL_Event Event;
        int mouseX = 0;
        int mouseY = 0;

        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_KEYDOWN)
            {
                switch (Event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    Running = 0;
                    break;

                case 'f':
                    FullScreen = !FullScreen;
                    if (FullScreen)
                    {
                        SDL_SetWindowFullscreen(Window, WindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }
                    else
                    {
                        SDL_SetWindowFullscreen(Window, WindowFlags);
                    }
                    break;
                default:
                    GameCamera.OnKeyboard(Event.key.keysym.sym);
                    break;
                }
            }
            // Handle mouse motion
            else if (Event.type == SDL_MOUSEMOTION) {
                mouseX = Event.motion.x;
                mouseY = Event.motion.y;
                int deltaX = Event.motion.xrel;
                int deltaY = Event.motion.yrel;
                std::cout << "Mouse moved: (" << mouseX << ", " << mouseY << "), DeltaX: " << deltaX << ", DeltaY: " << deltaY << std::endl;

                GameCamera.OnMouse(deltaX, deltaY);
            }
            else if (Event.type == SDL_QUIT)
            {
                Running = 0;
            }
        }

        glViewport(0, 0, WinWidth, WinHeight);
        glClearColor(bgcolor, bgcolor, bgcolor, 0.f);

        RenderSceneCB(Primitives);

        SDL_GL_SwapWindow(Window);
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
