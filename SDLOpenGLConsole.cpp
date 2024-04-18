
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <memory>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>


#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
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
#include "Mesh/Basic/Cube2.h"

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

glm::vec3 CameraPos(0.0f, 0.0f,9.0f);
glm::vec3 CameraTarget(0.0f, 0.0f, 1.0f);
glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);
Camera GameCamera(WinWidth, WinHeight, CameraPos, CameraTarget, CameraUp);

std::vector<Primitive *> Primitives;

float FOV = 45.0f;
float NearZ = 1.0f;
float FarZ = 40.0f;
float ar = (float)WinWidth / (float)(WinHeight);

static void RenderSceneCB( vector<Primitive *> &Primitives)
{
    float YRotationAngle = 1.0f;
    GameCamera.OnRender();

    Primitives[0]->GetTransform().Rotate(0.0f, YRotationAngle, 0.0f);
    Primitives[1]->GetTransform().Rotate(YRotationAngle, 0.0f, 0.0f);
    Primitives[2]->GetTransform().Rotate(YRotationAngle, YRotationAngle, 0.0f);
    Primitives[3]->GetTransform().Rotate(YRotationAngle, YRotationAngle, 0.0f);

    glm::mat4x4 View = GameCamera.GetMatrix();
    glm::mat4x4 Projection = glm::perspective(FOV, ar, NearZ, FarZ);
    glm::mat4x4 VP = Projection * View;

    for (auto& primitive : Primitives)
    {
        primitive->Update(VP);
    }
}

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // Depth buffering

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

    glEnable(GL_DEPTH_TEST);


    const std::string pVSFileName  = "./Shaders/shader.vs";
    const std::string pFSFileName  = "./Shaders/shader.fs";
    const std::string pFSFileName2 = "./Shaders/shader2.fs";
    const std::string pLightEffectedFragmentShader = "./Shaders/light_source.fs";
    const std::string pLightEffectedVertexShader = "./Shaders/light_source.vs";
    const std::string pLightSourceBaseFragmentShader = "./Shaders/light_source_base.fs";
    const std::string pLightSourceBaseVertexShader = "./Shaders/light_source_base.vs";
    const std::string pBasicDiffuseMaterialFragmentShader = "./Shaders/diffuse_material.fs";
    const std::string pBasicDiffuseMaterialVertexShader = "./Shaders/diffuse_material.vs";

    const std::string pMaterialFragmentShader = "./Shaders/material_struct.fs";


    const std::string pMaterialLightmapVertexShader = "./Shaders/material_lightmap.vs";
    const std::string pMaterialLightmapFragmentShader = "./Shaders/material_lightmap.fs";

    CompiledShaderProgram textureShader = shaderCompiler.CompileShaders(pVSFileName, pFSFileName);
    CompiledShaderProgram textureShader2 = shaderCompiler.CompileShaders(pVSFileName, pFSFileName2);
    CompiledShaderProgram diffuseShader = shaderCompiler.CompileShaders(pBasicDiffuseMaterialVertexShader, pBasicDiffuseMaterialFragmentShader);
    CompiledShaderProgram lightShader = shaderCompiler.CompileShaders(pLightSourceBaseVertexShader, pLightSourceBaseFragmentShader);
    CompiledShaderProgram materialShader = shaderCompiler.CompileShaders(pBasicDiffuseMaterialVertexShader, pMaterialFragmentShader);
    CompiledShaderProgram lightmapShader = shaderCompiler.CompileShaders(pMaterialLightmapVertexShader, pMaterialLightmapFragmentShader);

    float ambientStrength = 0.1f;
    float shininess = 64.f;

    glm::vec3 materialAmbient = glm::vec3(0.5f, 0.5f, 0.5f);

    CubeLightSource light;
    light.SetName("LightSource");
    light.AddShader(lightShader);
    light.SetPosition(0.0f, 1.0f, 0.0f);
    Primitives.push_back(&light);

    ModelObject cube;
    cube.SetName("Cube");
    cube.AddShader(textureShader);
    cube.SetModel(std::make_unique<Cube>());
    cube.SetTexture("gSampler", "bricks.jpg");
    cube.SetPosition(0.0, -1.0f, 0.0f);
    Primitives.push_back(&cube);

    ModelObject cube2;
    cube2.SetName("Cube2");
    cube2.AddShader(textureShader);
    cube2.SetModel(std::make_unique<Cube>());
    cube2.SetTexture("gSampler", "container.jpg");
    cube2.SetPosition(1.0, 0.0f, 0.0f);
    Primitives.push_back(&cube2);

    ModelObject cube3;
    cube3.SetName("CubeDiffuse");
    cube3.SetModel(std::make_unique<Cube2>());
    cube3.AddShader(diffuseShader);
    cube3.SetPosition(-1.0f, 0.0f, 1.0f);
    cube3.SetUniform("viewPos", &GameCamera.GetPosition());
    cube3.SetUniform("lightPos", &light.GetTransform().GetPosition());
    cube3.SetUniform("lightColor", &light.GetLightColorRef());
    cube3.SetUniform("objectColor", &light.GetObjectColorRef());
    Primitives.push_back(&cube3);

    ModelObject cube4;
    cube4.SetName("CubeMaterial");
    cube4.SetModel(std::make_unique<Cube2>());
    cube4.AddShader(materialShader);
    cube4.SetPosition(-2.0f, 0.0f, 1.0f);
    cube4.SetUniform("viewPos", &GameCamera.GetPosition());
    //
    cube4.SetUniform("light.position", &light.GetTransform().GetPosition());
    cube4.SetUniform("light.ambient", &light.GetLightColorRef());
    cube4.SetUniform("light.diffuse", &light.GetObjectColorRef());
    cube4.SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    //
    cube4.SetUniform("material.shininess", &shininess);
    cube4.SetUniform("material.ambient", &materialAmbient);
    cube4.SetUniform("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    cube4.SetUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    Primitives.push_back(&cube4);

    float ambient_strength = 0.1f;
    float diffuse_strength = 1.0f;
    float specular_strength = 1.0f;
    ModelObject cube5;
    cube5.SetName("LightMapCube");
    cube5.SetModel(std::make_unique<Cube2>());
    cube5.AddShader(lightmapShader);
    cube5.SetPosition(-2.0f, -2.0f, 1.0f);
    cube5.SetUniform("viewPos", &GameCamera.GetPosition());
    cube5.SetUniform("strengths.ambient", &ambient_strength);
    cube5.SetUniform("strengths.diffuse", &diffuse_strength);
    cube5.SetUniform("strengths.specular", &specular_strength);


    cube5.SetTexture("material.diffuse", "./Textures/container2.png");
    cube5.SetTexture("material.specular", "./Textures/container2_specular.png");
    cube5.SetUniform("material.shininess", &shininess);
    cube5.SetUniform("light.position", &light.GetTransform().GetPosition());
    cube5.SetUniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    cube5.SetUniform("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    cube5.SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    //
    
    Primitives.push_back(&cube5);

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

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        {
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::Text("Lightning");

            ImGui::ColorEdit3("Light Color", (float*)&light.GetLightColorRef());
            ImGui::ColorEdit3("Light Object Color", (float*)&light.GetObjectColorRef());
            ImGui::ColorEdit3("MaterialAmbient", (float*)&materialAmbient);

            ImGui::SliderFloat("Ambient Strength", &ambient_strength, 0.0f, 2.0f);
            ImGui::SliderFloat("Diffuse Strength", &diffuse_strength, 0.0f, 2.0f);
            ImGui::SliderFloat("Specular Strength", &specular_strength, 0.0f, 2.0f);
            ImGui::SliderFloat("Shininess", &shininess, 0.001f, 1.0f);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
