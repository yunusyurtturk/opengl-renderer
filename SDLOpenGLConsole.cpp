// SDLOpenGLConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

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
    GameCamera.OnRender();

   Primitives[0]->GetTransform().Rotate(0.0f, YRotationAngle, 0.0f);
   Primitives[1]->GetTransform().Rotate(YRotationAngle, 0.0f, 0.0f);
   Primitives[2]->GetTransform().Rotate(YRotationAngle, YRotationAngle, 0.0f);

    glm::mat4x4 View = GameCamera.GetMatrix();
    glm::mat4x4 Projection = glm::perspective(FOV, ar, NearZ, FarZ);
    glm::mat4x4 VP = Projection * View;

    for (auto& primitive : Primitives)
    {
        primitive->Update(VP);
    }

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

    const char* glsl_version = "#version 130";
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags WindowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* Window = SDL_CreateWindow("ImGui SDL2 OpenGL Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WinWidth, WinHeight, WindowFlags);

    assert(Window);
    SDL_GLContext Context = SDL_GL_CreateContext(Window);
    SDL_GL_MakeCurrent(Window, Context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(Window, Context);
    ImGui_ImplOpenGL3_Init(glsl_version);











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

    const std::string pLightEffectedFragmentShader = "./Shaders/light_source.fs";
    const std::string pLightEffectedVertexShader = "./Shaders/light_source.vs";

    const std::string pLightSourceBaseFragmentShader = "./Shaders/light_source_base.fs";
    const std::string pLightSourceBaseVertexShader = "./Shaders/light_source_base.vs";

    const std::string pBasicDiffuseMaterialFragmentShader = "./Shaders/diffuse_material.fs";
    const std::string pBasicDiffuseMaterialVertexShader = "./Shaders/diffuse_material.vs";

    CompiledShaderProgram textureShader = shaderCompiler.CompileShaders(pVSFileName, pFSFileName);
    CompiledShaderProgram textureShader2 = shaderCompiler.CompileShaders(pVSFileName, pFSFileName2);
    CompiledShaderProgram diffuseShader = shaderCompiler.CompileShaders(pBasicDiffuseMaterialVertexShader, pBasicDiffuseMaterialFragmentShader);
    CompiledShaderProgram lightShader = shaderCompiler.CompileShaders(pLightSourceBaseVertexShader, pLightSourceBaseFragmentShader);

    CubeLightSource Light;
    Light.SetName("LightSource");
    Light.AddShader(lightShader);
    Light.SetPosition(3.0, 3.0f, -4.0f);
    Primitives.push_back(&Light);

    ModelObject Cube;
    Cube.SetName("Cube");
    Cube.AddShader(textureShader);
    Cube.SetTexture("bricks.jpg");
    Cube.SetPosition(0.0, 0.0f, -2.0f);
    Primitives.push_back(&Cube);

    ModelObject Cube2;
    Cube2.SetName("Cube2");
    Cube2.AddShader(textureShader2);
    Cube2.SetTexture("container.jpg");
    Cube2.SetPosition(0.0, -1.0f, -2.0f);
    Primitives.push_back(&Cube2);

    ModelObject Cube3;
    Cube3.SetName("CubeDiffuse");
    Cube3.AddShader(diffuseShader);
    Cube3.SetUniform("lightPos", &Light.GetTransform().GetPosition());
    Cube3.SetUniform("lightColor", &Light.GetLightColorRef());
    Cube3.SetUniform("objectColor", &Light.GetObjectColorRef());

    Cube3.SetPosition(2.0, -2.0f, -3.0f);
    Primitives.push_back(&Cube3);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool show_demo_window = true;
    bool show_another_window = false;

    while (Running)
    {
        SDL_Event Event;
        int mouseX = 0;
        int mouseY = 0;

        while (SDL_PollEvent(&Event))
        {
            ImGui_ImplSDL2_ProcessEvent(&Event);
            if (Event.type == SDL_QUIT)
                Running = false;
            if (Event.type == SDL_WINDOWEVENT && Event.window.event == SDL_WINDOWEVENT_CLOSE && Event.window.windowID == SDL_GetWindowID(Window))
                Running = false;

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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        static float f = 0.0f;
        static float cx = 0.0f;
        static float cy = 0.0f;
        static float cz = 0.0f;

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        {
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::Text("Lightning");

            ImGui::ColorEdit3("Light Color", (float*)&Light.GetLightColorRef());
            ImGui::ColorEdit3("Light Object Color", (float*)&Light.GetObjectColorRef());

            for (auto& primitive : Primitives) {
                
                ImGui::SliderFloat((primitive->GetName() + "x").c_str(), &primitive->GetTransform().GetPosition().x, -10.0f, 10.0f);
                ImGui::SliderFloat((primitive->GetName() + "y").c_str(), &primitive->GetTransform().GetPosition().y, -10.0f, 10.0f);
                ImGui::SliderFloat((primitive->GetName() + "z").c_str(), &primitive->GetTransform().GetPosition().z, -10.0f, 10.0f);

            }       
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        RenderSceneCB(Primitives);

        SDL_GL_SwapWindow(Window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(Context);
    SDL_DestroyWindow(Window);
    SDL_Quit();

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
