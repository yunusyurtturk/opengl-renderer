#pragma once
#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 textureCoord;

    Vertex() 
    {
        pos = glm::vec3(0.0f, 0.0f, 0.0f);
        color = glm::vec3(0.0f, 0.0f, 0.0f);
        normal = glm::vec3(0.0f, 0.0f, 0.0f);
        textureCoord = glm::vec2(0.0f, 0.0f);
    }

    Vertex(float x, float y, float z)
    {
        initPos(x, y, z);

        float red = (float)rand() / (float)RAND_MAX;
        float green = (float)rand() / (float)RAND_MAX;
        float blue = (float)rand() / (float)RAND_MAX;
        initColor(red, green, blue);
        initTexture(x, y);
    }
    Vertex(float x, float y, float z, float r, float g, float b)
    {
        initPos(x, y, z);
        initColor(r, g, b);
        initTexture(x, y);
    }
    Vertex(float x, float y, float z, float r, float g, float b, float nx, float ny, float nz, float u, float v)
    {
        initPos(x, y, z);
        initColor(r, g, b);
        initNormals(nx, ny, nz);
        textureCoord = glm::vec2(u, v);
    }
    void initPos(float x, float y, float z)
    {
        pos = glm::vec3(x, y, z);
    }
    void initColor(float r, float g, float b)
    {
        color = glm::vec3(r, g, b);
    }
    void initNormals(float nx, float ny, float nz)
    {
        normal = glm::vec3(nx, ny, nz);
    }
    void initTexture(float u, float v)
    {
        textureCoord = glm::vec2(u, v);
    }
};